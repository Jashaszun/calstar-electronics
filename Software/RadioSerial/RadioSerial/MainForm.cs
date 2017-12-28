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

        public static MainForm Main_Form;
        public MainForm()
        {
            InitializeComponent();

            updatePortNames();

            replayFileDialog.InitialDirectory = Environment.CurrentDirectory;

            Main_Form = this;
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
                port.DataReceived += (s, e) => receiveSerialData(DateTime.Now.ToFileTimeUtc());
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

        Queue<Tuple<long, string>> telemetryFormReplayQueue = new Queue<Tuple<long, string>>();
        void receiveSerialData(long time, bool real = true, string fakeData = "")
        {
            if (real)
                buffer += port.ReadExisting();
            else buffer += fakeData;

            // write time and data to log
            if (real && logFile != null)
            {
                logFile.Write(time + ": ");
                logFile.WriteLine(string.Join(" ", buffer.ToCharArray().Select(ch =>
                {
                    return string.Format("{0:X2}", (int)ch);
                })));
                logFile.Flush();
            }

            string tempBuffer = buffer;
            lock (telemetryFormReplayQueue)
            {
                telemetryFormReplayQueue.Enqueue(new Tuple<long, string>(time, tempBuffer));
            }
            serialReadTextBox.tryInvoke(() =>
            {
                lock (telemetryFormReplayQueue)
                {
                    while (telemetryFormReplayQueue.Count > 0)
                    {
                        Tuple<long, string> item = telemetryFormReplayQueue.Dequeue();
                        serialReadTextBox.AppendText(item.Item2);
                        if (telemetryForm != null)
                            telemetryForm.receive(item.Item1, item.Item2);
                    }
                }
            });
            buffer = "";
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
            //telemetryButton.Visible = true;
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

                    this.tryInvoke(updateUIOnPortClose);
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
            //telemetryButton.Visible = false;
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
                //telemetryButton.Enabled = false;
                telemetryForm.FormClosed += (s, ev) =>
                {
                    telemetryForm = null;
                    //telemetryButton.Enabled = true;
                };
            }
            else
            {
                telemetryForm.BringToFront();
            }
        }

        private void retryCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (port != null)
            {
                port.Write(retryCheckBox.Checked ? "![YES_RETRY]!\n" : "![NO_RETRY]!\n");
            }
        }

        static Queue<Tuple<long, string>> replayQueue = new Queue<Tuple<long, string>>();
        public static void addReplayLine(long time, string replay)
        {
            lock(replayQueue)
            {
                replayQueue.Enqueue(new Tuple<long, string>(time, replay));
            }
            Main_Form.serialReadTextBox.tryInvoke(() =>
            {
                lock(replayQueue)
                {
                    while (replayQueue.Any())
                    {
                        Tuple<long, string> item = replayQueue.Dequeue();
                        Main_Form.receiveSerialData(item.Item1, false, item.Item2);
                    }
                }
            });
        }

        ReplayForm replayForm;
        private void replayFileButton_Click(object sender, EventArgs e)
        {
            if (replayForm == null)
            {
                if (replayFileDialog.ShowDialog() == DialogResult.OK)
                {
                    replayForm = new ReplayForm(replayFileDialog.FileName);
                    replayForm.FormClosed += (s, ev) =>
                    {
                        replayForm = null;
                        this.Text = "Radio Serial";
                    };
                    replayForm.Show();

                    this.Text = "Radio Serial (Replaying Log)";
                }
            }
            if (replayForm != null)
                replayForm.BringToFront();
        }
    }
}
