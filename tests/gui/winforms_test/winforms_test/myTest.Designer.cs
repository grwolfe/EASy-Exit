namespace winforms_test
{
    partial class myTest
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
            this.Hello_pushbutton = new System.Windows.Forms.Button();
            this.MyTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // Hello_pushbutton
            // 
            this.Hello_pushbutton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Hello_pushbutton.Location = new System.Drawing.Point(188, 99);
            this.Hello_pushbutton.Name = "Hello_pushbutton";
            this.Hello_pushbutton.Size = new System.Drawing.Size(107, 59);
            this.Hello_pushbutton.TabIndex = 0;
            this.Hello_pushbutton.Text = "Hello";
            this.Hello_pushbutton.UseVisualStyleBackColor = true;
            this.Hello_pushbutton.Click += new System.EventHandler(this.button1_Click);
            // 
            // MyTextBox
            // 
            this.MyTextBox.Location = new System.Drawing.Point(49, 187);
            this.MyTextBox.Name = "MyTextBox";
            this.MyTextBox.ReadOnly = true;
            this.MyTextBox.Size = new System.Drawing.Size(384, 22);
            this.MyTextBox.TabIndex = 1;
            this.MyTextBox.Tag = "myTextBox";
            this.MyTextBox.TextChanged += new System.EventHandler(this.MyTextBox_TextChanged);
            // 
            // myTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 285);
            this.Controls.Add(this.MyTextBox);
            this.Controls.Add(this.Hello_pushbutton);
            this.Name = "myTest";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Hello_pushbutton;
        private System.Windows.Forms.TextBox MyTextBox;
    }
}

