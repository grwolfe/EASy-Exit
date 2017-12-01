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

namespace gui
{
    public partial class EASy_EXIT : Form
    {
        public static readonly string[] BAUDRATES = { "9600", "115200" };
        public EASy_EXIT()
        {
            InitializeComponent();
            setAvailablePorts();
            setBaudSelect();
            setDefaultSettings();

            serial.DataReceived += new SerialDataReceivedEventHandler(serialDataReceived);
        }

        private void setAvailablePorts()
        {
            string[] ports = SerialPort.GetPortNames();
            portSelect.Items.AddRange(ports);
        }

        private void setBaudSelect()
        {
            baudSelect.Items.AddRange(BAUDRATES);
        }

        private void setDefaultSettings()
        {
            if (portSelect.Items.Count > 0)
                portSelect.SelectedIndex = 0;
            baudSelect.SelectedIndex = 0;
        }

        private void clear_textbox()
        {
            debugTextBox.Clear();
        }

        private void serialDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Invoke(new EventHandler(ReadSerialData));
        }

        private void ReadSerialData(object sender, EventArgs e)
        {
            // packets are newline terminated
            string data;
            try {
                data = serial.ReadExisting();
            } catch (System.IO.IOException E) {
                Console.WriteLine("{0} exception caught.", E);
                data = "ERROR READING FROM SERIAL\n";
            }
            debugTextBox.AppendText(data);
        }

        private void checkPortSettings()
        {
            bool portSelected = !string.IsNullOrWhiteSpace(portSelect.Text);
            bool baudSelected = !string.IsNullOrWhiteSpace(baudSelect.Text);

            if (portSelected && baudSelected)
                connectButton.Enabled = true;
            else
                connectButton.Enabled = false;
        }

        private void portSelect_SelectedIndexChanged(object sender, EventArgs e)
        { checkPortSettings(); }

        private void baudSelect_SelectedIndexChanged(object sender, EventArgs e)
        { checkPortSettings(); }

        private void scanButton_Click(object sender, EventArgs e)
        { setAvailablePorts(); }

        private void connectButton_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                disconnectSerialDevice();
                transmitDataText.Enabled = false;
                transmitSerial.Enabled = false;
                portSelect.Enabled = true;
                baudSelect.Enabled = true;
                scanButton.Enabled = true;
                connectButton.Text = "Connect";
            }
            else
            {
                connectSerialDevice();
                transmitSerial.Enabled = true;
                transmitDataText.Enabled = true;
                portSelect.Enabled = false;
                baudSelect.Enabled = false;
                scanButton.Enabled = false;
                connectButton.Text = "Disconnect";
                transmitDataText.Focus();
            }
        }

        private void disconnectSerialDevice()
        {
            if (serial.IsOpen)
                serial.Close();
        }

        private void connectSerialDevice()
        {
            serial.PortName = portSelect.SelectedItem.ToString();
            serial.BaudRate = Convert.ToInt32(baudSelect.SelectedItem);
            serial.Open();
        }

        private void transmitSerial_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                sendSerialData(transmitDataText.Text);
                transmitDataText.Clear();
            }
            transmitDataText.Focus();
        }

        private void sendSerialData(String data)
        {
            // append newline character as end of packet
            data += '\n';
            serial.Write(data);
        }

        private void EASy_EXIT_Load(object sender, EventArgs e)
        {

        }

        private void EAsy_EXIT_FormClosing(Object sender, FormClosedEventArgs e)
        { disconnectSerialDevice(); }

        private void transmitDataText_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == (int)Keys.Enter && serial.IsOpen)
            {
                sendSerialData(transmitDataText.Text);
                transmitDataText.Clear();
            }
        }

        private void clear_button_Click(object sender, EventArgs e)
        {
            clear_textbox();
        }
    }
}
