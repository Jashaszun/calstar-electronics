namespace RadioSerial
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.label3 = new System.Windows.Forms.Label();
            this.serialReadTextBox = new System.Windows.Forms.TextBox();
            this.clearRawButton = new System.Windows.Forms.Button();
            this.serialWriteTextBox = new System.Windows.Forms.TextBox();
            this.sendButton = new System.Windows.Forms.Button();
            this.sendTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.includeNewlinesCheckbox = new System.Windows.Forms.CheckBox();
            this.clearSentButton = new System.Windows.Forms.Button();
            this.retryCheckBox = new System.Windows.Forms.CheckBox();
            this.refreshPortsButton = new System.Windows.Forms.ToolStripMenuItem();
            this.portNamesBox = new System.Windows.Forms.ToolStripComboBox();
            this.baudRateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.baudRateBox = new System.Windows.Forms.ToolStripComboBox();
            this.connectButton = new System.Windows.Forms.ToolStripMenuItem();
            this.disconnectButton = new System.Windows.Forms.ToolStripMenuItem();
            this.telemetryButton = new System.Windows.Forms.ToolStripMenuItem();
            this.logFileTextBox = new System.Windows.Forms.ToolStripTextBox();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.menuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 31);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Raw Data:";
            // 
            // serialReadTextBox
            // 
            this.serialReadTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.serialReadTextBox.BackColor = System.Drawing.Color.Black;
            this.serialReadTextBox.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.serialReadTextBox.ForeColor = System.Drawing.Color.Lime;
            this.serialReadTextBox.Location = new System.Drawing.Point(12, 47);
            this.serialReadTextBox.Multiline = true;
            this.serialReadTextBox.Name = "serialReadTextBox";
            this.serialReadTextBox.ReadOnly = true;
            this.serialReadTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.serialReadTextBox.Size = new System.Drawing.Size(526, 236);
            this.serialReadTextBox.TabIndex = 3;
            // 
            // clearRawButton
            // 
            this.clearRawButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.clearRawButton.Location = new System.Drawing.Point(12, 289);
            this.clearRawButton.Name = "clearRawButton";
            this.clearRawButton.Size = new System.Drawing.Size(75, 23);
            this.clearRawButton.TabIndex = 5;
            this.clearRawButton.Text = "Clear";
            this.clearRawButton.UseVisualStyleBackColor = true;
            this.clearRawButton.Click += new System.EventHandler(this.clearRawButton_Click);
            // 
            // serialWriteTextBox
            // 
            this.serialWriteTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.serialWriteTextBox.BackColor = System.Drawing.Color.Black;
            this.serialWriteTextBox.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.serialWriteTextBox.ForeColor = System.Drawing.Color.Lime;
            this.serialWriteTextBox.Location = new System.Drawing.Point(544, 88);
            this.serialWriteTextBox.Multiline = true;
            this.serialWriteTextBox.Name = "serialWriteTextBox";
            this.serialWriteTextBox.ReadOnly = true;
            this.serialWriteTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.serialWriteTextBox.Size = new System.Drawing.Size(233, 195);
            this.serialWriteTextBox.TabIndex = 6;
            // 
            // sendButton
            // 
            this.sendButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.sendButton.Enabled = false;
            this.sendButton.Location = new System.Drawing.Point(733, 47);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(44, 23);
            this.sendButton.TabIndex = 7;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // sendTextBox
            // 
            this.sendTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.sendTextBox.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sendTextBox.Location = new System.Drawing.Point(544, 49);
            this.sendTextBox.Name = "sendTextBox";
            this.sendTextBox.Size = new System.Drawing.Size(183, 22);
            this.sendTextBox.TabIndex = 8;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(544, 72);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Sent:";
            // 
            // includeNewlinesCheckbox
            // 
            this.includeNewlinesCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.includeNewlinesCheckbox.AutoSize = true;
            this.includeNewlinesCheckbox.Checked = true;
            this.includeNewlinesCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.includeNewlinesCheckbox.Location = new System.Drawing.Point(655, 71);
            this.includeNewlinesCheckbox.Name = "includeNewlinesCheckbox";
            this.includeNewlinesCheckbox.Size = new System.Drawing.Size(122, 17);
            this.includeNewlinesCheckbox.TabIndex = 10;
            this.includeNewlinesCheckbox.Text = "Include Newlines ^^";
            this.includeNewlinesCheckbox.UseVisualStyleBackColor = true;
            // 
            // clearSentButton
            // 
            this.clearSentButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.clearSentButton.Location = new System.Drawing.Point(544, 289);
            this.clearSentButton.Name = "clearSentButton";
            this.clearSentButton.Size = new System.Drawing.Size(75, 23);
            this.clearSentButton.TabIndex = 11;
            this.clearSentButton.Text = "Clear";
            this.clearSentButton.UseVisualStyleBackColor = true;
            this.clearSentButton.Click += new System.EventHandler(this.clearSentButton_Click);
            // 
            // retryCheckBox
            // 
            this.retryCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.retryCheckBox.AutoSize = true;
            this.retryCheckBox.Checked = true;
            this.retryCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.retryCheckBox.Location = new System.Drawing.Point(598, 71);
            this.retryCheckBox.Name = "retryCheckBox";
            this.retryCheckBox.Size = new System.Drawing.Size(51, 17);
            this.retryCheckBox.TabIndex = 12;
            this.retryCheckBox.Text = "Retry";
            this.retryCheckBox.UseVisualStyleBackColor = true;
            this.retryCheckBox.CheckedChanged += new System.EventHandler(this.retryCheckBox_CheckedChanged);
            // 
            // refreshPortsButton
            // 
            this.refreshPortsButton.Name = "refreshPortsButton";
            this.refreshPortsButton.Size = new System.Drawing.Size(44, 23);
            this.refreshPortsButton.Text = "Port:";
            this.refreshPortsButton.Click += new System.EventHandler(this.portToolStripMenuItem_Click);
            // 
            // portNamesBox
            // 
            this.portNamesBox.Name = "portNamesBox";
            this.portNamesBox.Size = new System.Drawing.Size(121, 23);
            // 
            // baudRateToolStripMenuItem
            // 
            this.baudRateToolStripMenuItem.Enabled = false;
            this.baudRateToolStripMenuItem.Name = "baudRateToolStripMenuItem";
            this.baudRateToolStripMenuItem.Size = new System.Drawing.Size(75, 23);
            this.baudRateToolStripMenuItem.Text = "Baud Rate:";
            // 
            // baudRateBox
            // 
            this.baudRateBox.Items.AddRange(new object[] {
            "4800",
            "9600",
            "14400",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.baudRateBox.Name = "baudRateBox";
            this.baudRateBox.Size = new System.Drawing.Size(121, 23);
            this.baudRateBox.Text = "19200";
            // 
            // connectButton
            // 
            this.connectButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(64, 23);
            this.connectButton.Text = "Connect";
            this.connectButton.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // disconnectButton
            // 
            this.disconnectButton.BackColor = System.Drawing.Color.Red;
            this.disconnectButton.Name = "disconnectButton";
            this.disconnectButton.Size = new System.Drawing.Size(78, 23);
            this.disconnectButton.Text = "Disconnect";
            this.disconnectButton.Visible = false;
            this.disconnectButton.Click += new System.EventHandler(this.disconnectButton_Click);
            // 
            // telemetryButton
            // 
            this.telemetryButton.Name = "telemetryButton";
            this.telemetryButton.Size = new System.Drawing.Size(110, 23);
            this.telemetryButton.Text = "Attach Telemetry";
            this.telemetryButton.Visible = false;
            this.telemetryButton.Click += new System.EventHandler(this.telemetryButton_Click);
            // 
            // logFileTextBox
            // 
            this.logFileTextBox.Name = "logFileTextBox";
            this.logFileTextBox.Size = new System.Drawing.Size(100, 23);
            this.logFileTextBox.Text = "log_.txt";
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.refreshPortsButton,
            this.portNamesBox,
            this.baudRateToolStripMenuItem,
            this.baudRateBox,
            this.connectButton,
            this.disconnectButton,
            this.telemetryButton,
            this.logFileTextBox});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(789, 27);
            this.menuStrip.TabIndex = 2;
            this.menuStrip.Text = "menuStrip1";
            // 
            // MainForm
            // 
            this.AcceptButton = this.sendButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(789, 324);
            this.Controls.Add(this.retryCheckBox);
            this.Controls.Add(this.clearSentButton);
            this.Controls.Add(this.includeNewlinesCheckbox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.sendTextBox);
            this.Controls.Add(this.sendButton);
            this.Controls.Add(this.serialWriteTextBox);
            this.Controls.Add(this.clearRawButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.serialReadTextBox);
            this.Controls.Add(this.menuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip;
            this.Name = "MainForm";
            this.Text = "Radio Serial";
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox serialReadTextBox;
        private System.Windows.Forms.Button clearRawButton;
        private System.Windows.Forms.TextBox serialWriteTextBox;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.TextBox sendTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox includeNewlinesCheckbox;
        private System.Windows.Forms.Button clearSentButton;
        private System.Windows.Forms.CheckBox retryCheckBox;
        private System.Windows.Forms.ToolStripMenuItem refreshPortsButton;
        private System.Windows.Forms.ToolStripComboBox portNamesBox;
        private System.Windows.Forms.ToolStripMenuItem baudRateToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox baudRateBox;
        private System.Windows.Forms.ToolStripMenuItem connectButton;
        private System.Windows.Forms.ToolStripMenuItem disconnectButton;
        private System.Windows.Forms.ToolStripMenuItem telemetryButton;
        private System.Windows.Forms.ToolStripTextBox logFileTextBox;
        private System.Windows.Forms.MenuStrip menuStrip;
    }
}

