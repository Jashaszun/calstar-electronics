using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.IO.Ports;

namespace RadioSerial
{
    public partial class TelemetryForm : Form
    {
        List<Tuple<Double, Double>> altitudeData;
        List<Tuple<Double, Double, Double, Double>> accelData;

        SerialPort port;
        public TelemetryForm(SerialPort port)
        {
            InitializeComponent();
            this.port = port;
            startTime = DateTime.Now;
            altitudeData = new List<Tuple<Double, Double>>();
            accelData = new List<Tuple<double, double, double, double>>();
        }

        LineAnnotation maxLine;
        double maxAlt = double.MinValue;
        double maxAltTime = double.MinValue;
        DateTime startTime;
        double lastTimeInAltChart = -1000;
        void addAltitude(double time, double alt)
        {
            if (alt > maxAlt)
            {
                maxAlt = alt;
                maxAltTime = time;

                maxAltitudeLabel.Text = maxAlt + " ft";
                maxAltitudeTimeLabel.Text = maxAltTime + "s";

                if (maxLine == null)
                {
                    maxLine = new LineAnnotation()
                    {
                        IsSizeAlwaysRelative = false,
                        LineColor = Color.Blue,
                        LineWidth = 2,
                        X = 0,
                        Y = maxAlt,
                        Width = maxAltTime,
                        Height = 0,
                        AnchorDataPoint = altitudeChart.Series["Altitude"].Points[0]
                    };
                    altitudeChart.Annotations.Add(maxLine);
                }
                else
                {
                    maxLine.Y = maxAlt;
                    maxLine.Width = maxAltTime;
                }
            }

            if (altitudeData.Count == 0 || (time - lastTimeInAltChart >= .2))
            {
                // only add to the chart at most 5 times per second
                altitudeChart.Series["Altitude"].Points.AddXY(time, alt);
                altitudeChart.Invalidate();

                lastTimeInAltChart = time;
            }
            currentAltitudeLabel.Text = alt + " ft";
            altitudeData.Add(new Tuple<double, double>(time, alt));
        }

        double lastTimeInAccelChart = -1000;
        void addAcceleration(double time, double x, double y, double z)
        {
            if (time - lastTimeInAccelChart >= .2)
            {
                // only add to the chart at most 5 times per second
                accelChart.Series["Accel_X"].Points.AddXY(time, x);
                accelChart.Series["Accel_Y"].Points.AddXY(time, y);
                accelChart.Series["Accel_Z"].Points.AddXY(time, z);
                accelChart.Invalidate();

                lastTimeInAccelChart = time;
            }
            accelData.Add(new Tuple<double, double, double, double>(time, x, y, z));
        }

        string fullBuffer = "";
        string currentLine = "";
        public void receive(string buffer)
        {
            fullBuffer += buffer;
            foreach (char ch in buffer)
            {
                if (ch == '\n')
                {
                    processLine(currentLine);
                    currentLine = "";
                }
                else if (ch != '\r')
                {
                    currentLine += ch;
                }
            }
        }

        bool pulsingServo = false;
        private void processLine(string line)
        {
            if (line.StartsWith("![") && line.EndsWith("]!"))
            {
                line = line.Substring(2, line.Length - 4);

                if (line.StartsWith("RSSI="))
                {
                    rssiLabel.Text = "RSSI = " + line.Substring(5);
                }
            }
            else if (line == "S_OFF")
            {
                deplSignalDisplay.Text = "Off";
                deplSignalDisplay.BackColor = Color.Yellow;
            }
            else if (line == "S_ON")
            {
                deplSignalDisplay.Text = "Active";
                deplSignalDisplay.BackColor = Color.Green;
            }
            else if (line == "S_PULSE")
            {
                deplSignalDisplay.Text = "Active";
                deplSignalDisplay.BackColor = Color.Green;
                Timer t = new Timer();
                t.Interval = 2000;
                t.Tick += (s, e) => {
                    deplSignalDisplay.Text = "Off";
                    deplSignalDisplay.BackColor = Color.Yellow;
                    t.Stop();
                };
                t.Start();
            }
            else if (line.StartsWith("SERVO"))
            {
                if (line == "SERVO_PULSE")
                {
                    pulsingServo = true;
                    servoSpeedTrackBar.Value = 88;
                    servoSpeedTrackBar.Enabled = false;
                    servoSpeedLabel.Text = "Servo Speed: " + servoSpeedTrackBar.Value;
                    Timer t = new Timer();
                    t.Interval = 2000;
                    t.Tick += (s, e) => {
                        servoSpeedTrackBar.Value = 90;
                        servoSpeedTrackBar.Enabled = true;
                        servoSpeedLabel.Text = "Servo Speed: " + servoSpeedTrackBar.Value;
                        pulsingServo = false;
                        t.Stop();
                    };
                    t.Start();
                }
                else if (!pulsingServo)
                {
                    try
                    {
                        int value = int.Parse(line.Substring("SERVO".Length));
                        if (value >= 85 && value <= 95)
                        {
                            servoSpeedTrackBar.Value = value;
                            servoSpeedLabel.Text = "Servo Speed: " + servoSpeedTrackBar.Value;
                        }
                    }
                    catch (Exception) { }
                }
            }
            else if (line == "ALT_ZERO")
            {
                resetGraphsButton_Click(null, null);
            }
            else if (line.StartsWith("Ac: "))
            {
                try
                {
                    string[] parts = line.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    int scaledAccelX = int.Parse(parts[2]);
                    int scaledAccelY = int.Parse(parts[4]);
                    int scaledAccelZ = int.Parse(parts[6]);
                    double actualAccelX = scaledAccelX / 1000.0;
                    double actualAccelY = scaledAccelY / 1000.0;
                    double actualAccelZ = scaledAccelZ / 1000.0;

                    int scaledAlt = int.Parse(parts[8]);
                    double actualAlt = scaledAlt;// / 1000.0;

                    double time = DateTime.Now.Subtract(startTime).TotalSeconds;
                    addAltitude(time, actualAlt);
                    addAcceleration(time, actualAccelX - 1, actualAccelY, actualAccelZ);
                }
                catch (Exception) { }
            }
        }

        private void resetGraphsButton_Click(object sender, EventArgs e)
        {
            altitudeChart.Series["Altitude"].Points.Clear();
            altitudeChart.Annotations.Clear();
            maxLine = null;
            maxAlt = double.MinValue;
            maxAltTime = double.MinValue;
            maxAltitudeLabel.Text = "0 ft";
            maxAltitudeTimeLabel.Text = "0s";
            currentAltitudeLabel.Text = "0 ft";
            accelChart.Series["Accel_X"].Points.Clear();
            accelChart.Series["Accel_Y"].Points.Clear();
            accelChart.Series["Accel_Z"].Points.Clear();
            startTime = DateTime.Now;
            lastTimeInAltChart = -1000;
            lastTimeInAccelChart = -1000;
            altitudeData.Clear();
            accelData.Clear();
        }

        private void deplSigOffButton_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write("o\n");
            }
            catch (Exception) { }
        }

        private void deplSigOnButton_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write("a\n");
            }
            catch (Exception) { }
        }

        private void servoSpeedTrackBar_Scroll(object sender, EventArgs e)
        {
            try
            {
                if (!pulsingServo)
                    port.Write("0" + servoSpeedTrackBar.Value + "\n");
            }
            catch (Exception) { }
        }

        private void zeroAltitudeButton_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write("z\n");
            }
            catch (Exception) { }
        }

        private void deplSignalPulseButton_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write("p\n");
            }
            catch (Exception) { }
        }

        private void pulseServoButton_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write("e\n");
            }
            catch (Exception) { }
        }
    }
}
