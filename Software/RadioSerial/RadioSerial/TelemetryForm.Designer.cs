namespace RadioSerial
{
    partial class TelemetryForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TelemetryForm));
            this.altitudeChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.label1 = new System.Windows.Forms.Label();
            this.maxAltitudeLabel = new System.Windows.Forms.Label();
            this.maxAltitudeTimeLabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.accelChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.deplSignalDisplay = new System.Windows.Forms.Label();
            this.servoSpeedTrackBar = new System.Windows.Forms.TrackBar();
            this.servoSpeedLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.rssiLabel = new System.Windows.Forms.Label();
            this.resetGraphsButton = new System.Windows.Forms.Button();
            this.currentAltitudeLabel = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.deplSigOffButton = new System.Windows.Forms.Button();
            this.deplSigOnButton = new System.Windows.Forms.Button();
            this.zeroAltitudeButton = new System.Windows.Forms.Button();
            this.deplSignalPulseButton = new System.Windows.Forms.Button();
            this.pulseServoButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.altitudeChart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.accelChart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.servoSpeedTrackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // altitudeChart
            // 
            this.altitudeChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.ArrowStyle = System.Windows.Forms.DataVisualization.Charting.AxisArrowStyle.Triangle;
            chartArea1.AxisX.Minimum = 0D;
            chartArea1.Name = "ChartArea1";
            this.altitudeChart.ChartAreas.Add(chartArea1);
            this.altitudeChart.Location = new System.Drawing.Point(12, 12);
            this.altitudeChart.Name = "altitudeChart";
            series1.BackHatchStyle = System.Windows.Forms.DataVisualization.Charting.ChartHatchStyle.BackwardDiagonal;
            series1.BorderColor = System.Drawing.Color.Red;
            series1.BorderWidth = 2;
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.SplineArea;
            series1.IsVisibleInLegend = false;
            series1.Name = "Altitude";
            series1.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            this.altitudeChart.Series.Add(series1);
            this.altitudeChart.Size = new System.Drawing.Size(557, 214);
            this.altitudeChart.TabIndex = 0;
            this.altitudeChart.Text = "chart1";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(575, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(96, 19);
            this.label1.TabIndex = 1;
            this.label1.Text = "Max Altitude:";
            // 
            // maxAltitudeLabel
            // 
            this.maxAltitudeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.maxAltitudeLabel.AutoSize = true;
            this.maxAltitudeLabel.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.maxAltitudeLabel.Location = new System.Drawing.Point(591, 31);
            this.maxAltitudeLabel.Name = "maxAltitudeLabel";
            this.maxAltitudeLabel.Size = new System.Drawing.Size(27, 19);
            this.maxAltitudeLabel.TabIndex = 2;
            this.maxAltitudeLabel.Text = "0ft";
            // 
            // maxAltitudeTimeLabel
            // 
            this.maxAltitudeTimeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.maxAltitudeTimeLabel.AutoSize = true;
            this.maxAltitudeTimeLabel.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.maxAltitudeTimeLabel.Location = new System.Drawing.Point(591, 69);
            this.maxAltitudeTimeLabel.Name = "maxAltitudeTimeLabel";
            this.maxAltitudeTimeLabel.Size = new System.Drawing.Size(24, 19);
            this.maxAltitudeTimeLabel.TabIndex = 4;
            this.maxAltitudeTimeLabel.Text = "0s";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(575, 50);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(62, 19);
            this.label4.TabIndex = 3;
            this.label4.Text = "at Time:";
            // 
            // accelChart
            // 
            this.accelChart.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea2.AxisX.ArrowStyle = System.Windows.Forms.DataVisualization.Charting.AxisArrowStyle.Triangle;
            chartArea2.AxisX.Minimum = 0D;
            chartArea2.Name = "ChartArea1";
            this.accelChart.ChartAreas.Add(chartArea2);
            legend1.Name = "Legend1";
            this.accelChart.Legends.Add(legend1);
            this.accelChart.Location = new System.Drawing.Point(12, 232);
            this.accelChart.Name = "accelChart";
            series2.BorderColor = System.Drawing.Color.Red;
            series2.BorderWidth = 2;
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series2.Legend = "Legend1";
            series2.Name = "Accel_X";
            series2.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            series2.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            series3.BorderColor = System.Drawing.Color.Lime;
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series3.Legend = "Legend1";
            series3.Name = "Accel_Y";
            series4.BorderColor = System.Drawing.Color.Blue;
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series4.Legend = "Legend1";
            series4.Name = "Accel_Z";
            this.accelChart.Series.Add(series2);
            this.accelChart.Series.Add(series3);
            this.accelChart.Series.Add(series4);
            this.accelChart.Size = new System.Drawing.Size(557, 300);
            this.accelChart.TabIndex = 5;
            this.accelChart.Text = "chart1";
            // 
            // deplSignalDisplay
            // 
            this.deplSignalDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.deplSignalDisplay.BackColor = System.Drawing.Color.Yellow;
            this.deplSignalDisplay.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.deplSignalDisplay.Location = new System.Drawing.Point(575, 303);
            this.deplSignalDisplay.Name = "deplSignalDisplay";
            this.deplSignalDisplay.Size = new System.Drawing.Size(113, 23);
            this.deplSignalDisplay.TabIndex = 8;
            this.deplSignalDisplay.Text = "Off";
            this.deplSignalDisplay.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // servoSpeedTrackBar
            // 
            this.servoSpeedTrackBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.servoSpeedTrackBar.LargeChange = 1;
            this.servoSpeedTrackBar.Location = new System.Drawing.Point(574, 352);
            this.servoSpeedTrackBar.Maximum = 95;
            this.servoSpeedTrackBar.Minimum = 85;
            this.servoSpeedTrackBar.Name = "servoSpeedTrackBar";
            this.servoSpeedTrackBar.Size = new System.Drawing.Size(114, 45);
            this.servoSpeedTrackBar.TabIndex = 9;
            this.servoSpeedTrackBar.Value = 90;
            this.servoSpeedTrackBar.Scroll += new System.EventHandler(this.servoSpeedTrackBar_Scroll);
            // 
            // servoSpeedLabel
            // 
            this.servoSpeedLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.servoSpeedLabel.AutoSize = true;
            this.servoSpeedLabel.Location = new System.Drawing.Point(575, 336);
            this.servoSpeedLabel.Name = "servoSpeedLabel";
            this.servoSpeedLabel.Size = new System.Drawing.Size(87, 13);
            this.servoSpeedLabel.TabIndex = 10;
            this.servoSpeedLabel.Text = "Servo Speed: 90";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(577, 383);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(19, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "85";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(622, 383);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(19, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "90";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(665, 384);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(19, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "95";
            // 
            // rssiLabel
            // 
            this.rssiLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.rssiLabel.AutoSize = true;
            this.rssiLabel.Location = new System.Drawing.Point(604, 467);
            this.rssiLabel.Name = "rssiLabel";
            this.rssiLabel.Size = new System.Drawing.Size(53, 13);
            this.rssiLabel.TabIndex = 14;
            this.rssiLabel.Text = "RSSI: -60";
            // 
            // resetGraphsButton
            // 
            this.resetGraphsButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.resetGraphsButton.Location = new System.Drawing.Point(575, 139);
            this.resetGraphsButton.Name = "resetGraphsButton";
            this.resetGraphsButton.Size = new System.Drawing.Size(114, 23);
            this.resetGraphsButton.TabIndex = 15;
            this.resetGraphsButton.Text = "Reset Graphs";
            this.resetGraphsButton.UseVisualStyleBackColor = true;
            this.resetGraphsButton.Click += new System.EventHandler(this.resetGraphsButton_Click);
            // 
            // currentAltitudeLabel
            // 
            this.currentAltitudeLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.currentAltitudeLabel.AutoSize = true;
            this.currentAltitudeLabel.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.currentAltitudeLabel.Location = new System.Drawing.Point(591, 107);
            this.currentAltitudeLabel.Name = "currentAltitudeLabel";
            this.currentAltitudeLabel.Size = new System.Drawing.Size(27, 19);
            this.currentAltitudeLabel.TabIndex = 17;
            this.currentAltitudeLabel.Text = "0ft";
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(575, 88);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(116, 19);
            this.label7.TabIndex = 16;
            this.label7.Text = "Current Altitude:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(578, 232);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(98, 13);
            this.label6.TabIndex = 18;
            this.label6.Text = "Deployment Signal:";
            // 
            // deplSigOffButton
            // 
            this.deplSigOffButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.deplSigOffButton.Location = new System.Drawing.Point(575, 248);
            this.deplSigOffButton.Name = "deplSigOffButton";
            this.deplSigOffButton.Size = new System.Drawing.Size(45, 23);
            this.deplSigOffButton.TabIndex = 19;
            this.deplSigOffButton.Text = "Off";
            this.deplSigOffButton.UseVisualStyleBackColor = true;
            this.deplSigOffButton.Click += new System.EventHandler(this.deplSigOffButton_Click);
            // 
            // deplSigOnButton
            // 
            this.deplSigOnButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.deplSigOnButton.Location = new System.Drawing.Point(625, 248);
            this.deplSigOnButton.Name = "deplSigOnButton";
            this.deplSigOnButton.Size = new System.Drawing.Size(63, 23);
            this.deplSigOnButton.TabIndex = 20;
            this.deplSigOnButton.Text = "Active";
            this.deplSigOnButton.UseVisualStyleBackColor = true;
            this.deplSigOnButton.Click += new System.EventHandler(this.deplSigOnButton_Click);
            // 
            // zeroAltitudeButton
            // 
            this.zeroAltitudeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.zeroAltitudeButton.Location = new System.Drawing.Point(576, 168);
            this.zeroAltitudeButton.Name = "zeroAltitudeButton";
            this.zeroAltitudeButton.Size = new System.Drawing.Size(114, 23);
            this.zeroAltitudeButton.TabIndex = 21;
            this.zeroAltitudeButton.Text = "Zero Altitude";
            this.zeroAltitudeButton.UseVisualStyleBackColor = true;
            this.zeroAltitudeButton.Click += new System.EventHandler(this.zeroAltitudeButton_Click);
            // 
            // deplSignalPulseButton
            // 
            this.deplSignalPulseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.deplSignalPulseButton.Location = new System.Drawing.Point(575, 276);
            this.deplSignalPulseButton.Name = "deplSignalPulseButton";
            this.deplSignalPulseButton.Size = new System.Drawing.Size(113, 23);
            this.deplSignalPulseButton.TabIndex = 22;
            this.deplSignalPulseButton.Text = "Pulse";
            this.deplSignalPulseButton.UseVisualStyleBackColor = true;
            this.deplSignalPulseButton.Click += new System.EventHandler(this.deplSignalPulseButton_Click);
            // 
            // pulseServoButton
            // 
            this.pulseServoButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.pulseServoButton.Location = new System.Drawing.Point(574, 403);
            this.pulseServoButton.Name = "pulseServoButton";
            this.pulseServoButton.Size = new System.Drawing.Size(113, 23);
            this.pulseServoButton.TabIndex = 23;
            this.pulseServoButton.Text = "Pulse Servo (88)";
            this.pulseServoButton.UseVisualStyleBackColor = true;
            this.pulseServoButton.Click += new System.EventHandler(this.pulseServoButton_Click);
            // 
            // TelemetryForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(693, 542);
            this.Controls.Add(this.pulseServoButton);
            this.Controls.Add(this.deplSignalPulseButton);
            this.Controls.Add(this.zeroAltitudeButton);
            this.Controls.Add(this.deplSigOnButton);
            this.Controls.Add(this.deplSigOffButton);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.currentAltitudeLabel);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.resetGraphsButton);
            this.Controls.Add(this.rssiLabel);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.servoSpeedLabel);
            this.Controls.Add(this.servoSpeedTrackBar);
            this.Controls.Add(this.deplSignalDisplay);
            this.Controls.Add(this.accelChart);
            this.Controls.Add(this.maxAltitudeTimeLabel);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.maxAltitudeLabel);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.altitudeChart);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "TelemetryForm";
            this.Text = "Arktos Telemetry";
            ((System.ComponentModel.ISupportInitialize)(this.altitudeChart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.accelChart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.servoSpeedTrackBar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart altitudeChart;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label maxAltitudeLabel;
        private System.Windows.Forms.Label maxAltitudeTimeLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DataVisualization.Charting.Chart accelChart;
        private System.Windows.Forms.Label deplSignalDisplay;
        private System.Windows.Forms.TrackBar servoSpeedTrackBar;
        private System.Windows.Forms.Label servoSpeedLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label rssiLabel;
        private System.Windows.Forms.Button resetGraphsButton;
        private System.Windows.Forms.Label currentAltitudeLabel;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button deplSigOffButton;
        private System.Windows.Forms.Button deplSigOnButton;
        private System.Windows.Forms.Button zeroAltitudeButton;
        private System.Windows.Forms.Button deplSignalPulseButton;
        private System.Windows.Forms.Button pulseServoButton;
    }
}