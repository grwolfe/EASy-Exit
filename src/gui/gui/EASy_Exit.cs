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
        public static readonly String[] BAUDRATES = { "9600", "115200" };
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
            String[] ports = SerialPort.GetPortNames();
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
            baudSelect.SelectedIndex = 1;
        }

        private void serialDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Invoke(new EventHandler(EchoSerialData));
        }

        private void EchoSerialData(object sender, EventArgs e)
        {
            debugTextBox.AppendText(serial.ReadExisting());
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
        {
            checkPortSettings();
        }

        private void baudSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            checkPortSettings();
        }

        private void scanButton_Click(object sender, EventArgs e)
        {
            setAvailablePorts();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            serial.PortName = portSelect.SelectedItem.ToString();
            serial.BaudRate = Convert.ToInt32(baudSelect.SelectedItem);
            serial.Open();
            disconnectButton.Enabled = true;
            transmitSerial.Enabled = true;
            transmitDataText.Enabled = true;
            connectButton.Enabled = false;
            portSelect.Enabled = false;
            baudSelect.Enabled = false;
            scanButton.Enabled = false;
            transmitDataText.Focus();
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            disconnectSerialDevice();
            disconnectButton.Enabled = false;
            transmitDataText.Enabled = false;
            transmitSerial.Enabled = false;
            connectButton.Enabled = true;
            portSelect.Enabled = true;
            baudSelect.Enabled = true;
            scanButton.Enabled = true;
        }

        private void disconnectSerialDevice()
        {
            if (serial.IsOpen)
                serial.Close();
        }

        private void transmitSerial_Click(object sender, EventArgs e)
        {
            if (serial.IsOpen)
            {
                sendSerialData(transmitDataText.Text);
                transmitDataText.Text = "";
            }
            transmitDataText.Focus();
        }

        private void sendSerialData(String data)
        {
            serial.Write(data);
        }

        private void EASy_EXIT_Load(object sender, EventArgs e)
        {

        }

        private void EAsy_EXIT_FormClosing(Object sender, FormClosedEventArgs e)
        {
            disconnectSerialDevice();
        }
    }
}
