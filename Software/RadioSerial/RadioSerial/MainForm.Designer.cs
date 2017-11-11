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
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.refreshPortsButton = new System.Windows.Forms.ToolStripMenuItem();
            this.portNamesBox = new System.Windows.Forms.ToolStripComboBox();
            this.baudRateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.baudRateBox = new System.Windows.Forms.ToolStripComboBox();
            this.connectButton = new System.Windows.Forms.ToolStripMenuItem();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.disconnectButton = new System.Windows.Forms.ToolStripMenuItem();
            this.clearRawButton = new System.Windows.Forms.Button();
            this.menuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.refreshPortsButton,
            this.portNamesBox,
            this.baudRateToolStripMenuItem,
            this.baudRateBox,
            this.connectButton,
            this.disconnectButton});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(556, 27);
            this.menuStrip.TabIndex = 2;
            this.menuStrip.Text = "menuStrip1";
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
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 31);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Raw Data:";
            // 
            // textBox1
            // 
            this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox1.BackColor = System.Drawing.Color.Black;
            this.textBox1.ForeColor = System.Drawing.Color.Lime;
            this.textBox1.Location = new System.Drawing.Point(12, 47);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(532, 236);
            this.textBox1.TabIndex = 3;
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(556, 324);
            this.Controls.Add(this.clearRawButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.Name = "Form1";
            this.Text = "Radio Serial";
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem refreshPortsButton;
        private System.Windows.Forms.ToolStripComboBox portNamesBox;
        private System.Windows.Forms.ToolStripMenuItem baudRateToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox baudRateBox;
        private System.Windows.Forms.ToolStripMenuItem connectButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ToolStripMenuItem disconnectButton;
        private System.Windows.Forms.Button clearRawButton;
    }
}

