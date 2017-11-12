namespace gui
{
    partial class EASy_EXIT
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
            this.components = new System.ComponentModel.Container();
            this.serial = new System.IO.Ports.SerialPort(this.components);
            this.portSelect = new System.Windows.Forms.ComboBox();
            this.scanButton = new System.Windows.Forms.Button();
            this.baudSelect = new System.Windows.Forms.ComboBox();
            this.portSelectLabel = new System.Windows.Forms.TextBox();
            this.baudSelectLabel = new System.Windows.Forms.TextBox();
            this.connectButton = new System.Windows.Forms.Button();
            this.debugTextBox = new System.Windows.Forms.TextBox();
            this.transmitDataText = new System.Windows.Forms.TextBox();
            this.transmitSerial = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // serial
            // 
            this.serial.BaudRate = 115200;
            // 
            // portSelect
            // 
            this.portSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.portSelect.FormattingEnabled = true;
            this.portSelect.Location = new System.Drawing.Point(45, 39);
            this.portSelect.Name = "portSelect";
            this.portSelect.Size = new System.Drawing.Size(121, 24);
            this.portSelect.TabIndex = 0;
            this.portSelect.SelectedIndexChanged += new System.EventHandler(this.portSelect_SelectedIndexChanged);
            // 
            // scanButton
            // 
            this.scanButton.Location = new System.Drawing.Point(297, 38);
            this.scanButton.Name = "scanButton";
            this.scanButton.Size = new System.Drawing.Size(78, 25);
            this.scanButton.TabIndex = 1;
            this.scanButton.Text = "Scan";
            this.scanButton.UseVisualStyleBackColor = true;
            this.scanButton.Click += new System.EventHandler(this.scanButton_Click);
            // 
            // baudSelect
            // 
            this.baudSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.baudSelect.FormattingEnabled = true;
            this.baudSelect.Location = new System.Drawing.Point(170, 39);
            this.baudSelect.Name = "baudSelect";
            this.baudSelect.Size = new System.Drawing.Size(121, 24);
            this.baudSelect.TabIndex = 2;
            this.baudSelect.SelectedIndexChanged += new System.EventHandler(this.baudSelect_SelectedIndexChanged);
            // 
            // portSelectLabel
            // 
            this.portSelectLabel.BackColor = System.Drawing.SystemColors.Control;
            this.portSelectLabel.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.portSelectLabel.Location = new System.Drawing.Point(45, 18);
            this.portSelectLabel.Name = "portSelectLabel";
            this.portSelectLabel.Size = new System.Drawing.Size(133, 15);
            this.portSelectLabel.TabIndex = 3;
            this.portSelectLabel.Text = "Select Device Port";
            // 
            // baudSelectLabel
            // 
            this.baudSelectLabel.BackColor = System.Drawing.SystemColors.Control;
            this.baudSelectLabel.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.baudSelectLabel.Location = new System.Drawing.Point(170, 18);
            this.baudSelectLabel.Name = "baudSelectLabel";
            this.baudSelectLabel.Size = new System.Drawing.Size(121, 15);
            this.baudSelectLabel.TabIndex = 4;
            this.baudSelectLabel.Text = "Select Baud Rate";
            // 
            // connectButton
            // 
            this.connectButton.Enabled = false;
            this.connectButton.Location = new System.Drawing.Point(381, 26);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(101, 48);
            this.connectButton.TabIndex = 5;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // debugTextBox
            // 
            this.debugTextBox.Location = new System.Drawing.Point(43, 145);
            this.debugTextBox.Multiline = true;
            this.debugTextBox.Name = "debugTextBox";
            this.debugTextBox.ReadOnly = true;
            this.debugTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.debugTextBox.Size = new System.Drawing.Size(475, 241);
            this.debugTextBox.TabIndex = 7;
            this.debugTextBox.WordWrap = false;
            // 
            // transmitDataText
            // 
            this.transmitDataText.AcceptsReturn = true;
            this.transmitDataText.Location = new System.Drawing.Point(43, 97);
            this.transmitDataText.MaxLength = 256;
            this.transmitDataText.Name = "transmitDataText";
            this.transmitDataText.Size = new System.Drawing.Size(330, 22);
            this.transmitDataText.TabIndex = 8;
            this.transmitDataText.KeyDown += new System.Windows.Forms.KeyEventHandler(this.transmitDataText_KeyDown);
            // 
            // transmitSerial
            // 
            this.transmitSerial.Enabled = false;
            this.transmitSerial.Location = new System.Drawing.Point(381, 90);
            this.transmitSerial.Name = "transmitSerial";
            this.transmitSerial.Size = new System.Drawing.Size(96, 37);
            this.transmitSerial.TabIndex = 9;
            this.transmitSerial.Text = "Send";
            this.transmitSerial.UseVisualStyleBackColor = true;
            this.transmitSerial.Click += new System.EventHandler(this.transmitSerial_Click);
            // 
            // EASy_EXIT
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(548, 421);
            this.Controls.Add(this.transmitSerial);
            this.Controls.Add(this.transmitDataText);
            this.Controls.Add(this.debugTextBox);
            this.Controls.Add(this.connectButton);
            this.Controls.Add(this.baudSelectLabel);
            this.Controls.Add(this.portSelectLabel);
            this.Controls.Add(this.baudSelect);
            this.Controls.Add(this.scanButton);
            this.Controls.Add(this.portSelect);
            this.Name = "EASy_EXIT";
            this.Text = "EASy Exit System Interface";
            this.Load += new System.EventHandler(this.EASy_EXIT_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serial;
        private System.Windows.Forms.ComboBox portSelect;
        private System.Windows.Forms.Button scanButton;
        private System.Windows.Forms.ComboBox baudSelect;
        private System.Windows.Forms.TextBox portSelectLabel;
        private System.Windows.Forms.TextBox baudSelectLabel;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.TextBox debugTextBox;
        private System.Windows.Forms.TextBox transmitDataText;
        private System.Windows.Forms.Button transmitSerial;
    }
}

