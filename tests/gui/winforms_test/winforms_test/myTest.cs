using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winforms_test
{
    public partial class myTest : Form
    {
        public myTest()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MyTextBox.Text = "Hello!";
        }

        private void MyTextBox_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
