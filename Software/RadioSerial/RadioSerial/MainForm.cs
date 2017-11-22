using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace RadioSerial
{
    public partial class MainForm : Form
    {
        void updatePortNames()
        {
            string[] ports = SerialPort.GetPortNames();

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
        bool connect(string comPort, int baud)
        {
            try
            {
                port = new SerialPort(comPort, baud);
                buffer = "";
                port.DataReceived += (s, e) =>
                {
                    buffer += port.ReadExisting();
                    if (serialReadTextBox.InvokeRequired)
                    {
                        string tempBuffer = buffer;
                        serialReadTextBox.Invoke((Action)delegate { serialReadTextBox.AppendText(tempBuffer); });
                    }
                    else
                        serialReadTextBox.AppendText(buffer);
                    buffer = "";
                };
                port.Open();
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
            portNamesBox.Enabled = false;
            baudRateBox.Enabled = false;
            refreshPortsButton.Enabled = false;
            sendButton.Enabled = true;
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            if (port != null)
                port.Close();
            port = null;

            connectButton.Text = "Connect";
            connectButton.Enabled = true;
            disconnectButton.Visible = false;
            portNamesBox.Enabled = true;
            baudRateBox.Enabled = true;
            refreshPortsButton.Enabled = true;
            sendButton.Enabled = false;
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
    }
}
