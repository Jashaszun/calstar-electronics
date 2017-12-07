using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;

namespace RadioSerial
{
    public partial class MainForm : Form
    {
        void updatePortNames()
        {
            string[] ports = SerialPort.GetPortNames()
                .OrderBy(s => int.Parse(s.Substring("COM".Length))).ToArray();

            string selected = (string)portNamesBox.SelectedItem;
            portNamesBox.Items.Clear();
            portNamesBox.Items.AddRange(ports);
            if (selected != null && ports.Contains(selected))
            {
                portNamesBox.SelectedItem = selected;
                portNamesBox.Text = selected;
            }
            else if (portNamesBox.Items.Count > 0)
            {
                portNamesBox.SelectedIndex = 0;
                portNamesBox.Text = ports[0];
            }
            else
            {
                portNamesBox.Text = "";
            }
            portNamesBox.SelectedText = "";
        }

        public MainForm()
        {
            InitializeComponent();

            updatePortNames();
        }

        string buffer;
        SerialPort port;
        StreamWriter logFile;
        bool connect(string comPort, int baud)
        {
            try
            {
                logFile = logFileTextBox.Text != "" ? new StreamWriter(File.OpenWrite(logFileTextBox.Text))
                {
                    AutoFlush = true
                } : null;
                port = new SerialPort(comPort, baud);
                buffer = "";
                port.DataReceived += (s, e) =>
                {
                    buffer += port.ReadExisting();

                    // write time and data to log
                    if (logFile != null)
                    {
                        logFile.Write(DateTime.Now.ToFileTimeUtc() + ": ");
                        logFile.WriteLine(string.Join(" ", buffer.ToCharArray().Select(ch =>
                        {
                            return string.Format("{0:X2}", (int)ch);
                        })));
                        logFile.Flush();
                    }
                    
                    if (serialReadTextBox.InvokeRequired)
                    {
                        string tempBuffer = buffer;
                        serialReadTextBox.Invoke((Action)delegate { serialReadTextBox.AppendText(tempBuffer); });
                        if (telemetryForm != null)
                        {
                            telemetryForm.Invoke((Action)delegate {
                                try
                                {
                                    telemetryForm.receive(tempBuffer);
                                }
                                catch (Exception)
                                {

                                }
                            });
                        }   
                    }
                    else
                        serialReadTextBox.AppendText(buffer);
                    buffer = "";
                };
                port.Open();
                port.Write(retryCheckBox.Checked ? "![YES_RETRY]!\n" : "![NO_RETRY]!\n");
                return true;
            }
            catch (Exception)
            {
                port = null;
                return false;
            }
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int baud;
            if (!int.TryParse(baudRateBox.Text, out baud))
            {
                MessageBox.Show("'" + baudRateBox.Text + "' is not a baud rate.", "Pick a valid baud rate.");
                return;
            }
            if (!connect(portNamesBox.Text, baud))
            {
                MessageBox.Show("Could not connect.");
                return;
            }

            connectButton.Text = "Connected...";
            connectButton.Enabled = false;
            disconnectButton.Visible = true;
            disconnectButton.Enabled = true;
            portNamesBox.Enabled = false;
            baudRateBox.Enabled = false;
            refreshPortsButton.Enabled = false;
            sendButton.Enabled = true;
            telemetryButton.Visible = true;
            logFileTextBox.Enabled = false;
            if (logFileTextBox.Text == "")
                logFileTextBox.Visible = false;
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            if (port != null)
            {
                disconnectButton.Enabled = false;
                Thread portClose = new Thread(new ThreadStart(() =>
                {
                    port.Close();
                    port = null;
                    if (logFile != null)
                        logFile.Close();

                    if (InvokeRequired)
                    {
                        Invoke((Action)delegate
                        {
                            updateUIOnPortClose();
                        });
                    }
                    else updateUIOnPortClose();
                })); //close port in new thread to avoid hang

                portClose.Start(); // close port in new thread to avoid hanging
            }
        }
        private void updateUIOnPortClose()
        {
            connectButton.Text = "Connect";
            connectButton.Enabled = true;
            disconnectButton.Visible = false;
            portNamesBox.Enabled = true;
            baudRateBox.Enabled = true;
            refreshPortsButton.Enabled = true;
            sendButton.Enabled = false;
            telemetryButton.Visible = false;
            logFileTextBox.Enabled = true;
            logFileTextBox.Visible = true;
        }

        private void clearRawButton_Click(object sender, EventArgs e)
        {
            serialReadTextBox.Clear();
        }

        private void portToolStripMenuItem_Click(object sender, EventArgs e)
        {
            updatePortNames();
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            port.Write(sendTextBox.Text + (includeNewlinesCheckbox.Checked ? "\n" :""));
            serialWriteTextBox.AppendText("» " + sendTextBox.Text + "\r\n");
            sendTextBox.Text = "";
        }

        private void clearSentButton_Click(object sender, EventArgs e)
        {
            serialWriteTextBox.Clear();
        }

        TelemetryForm telemetryForm;
        private void telemetryButton_Click(object sender, EventArgs e)
        {
            if (telemetryForm == null)
            {
                telemetryForm = new TelemetryForm(port);
                telemetryForm.Show();
                telemetryButton.Enabled = false;
                telemetryForm.FormClosed += (s, ev) =>
                {
                    telemetryForm = null;
                    telemetryButton.Enabled = true;
                };
            }
        }

        private void retryCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (port != null)
            {
                port.Write(retryCheckBox.Checked ? "![YES_RETRY]!\n" : "![NO_RETRY]!\n");
            }
        }
    }
}
