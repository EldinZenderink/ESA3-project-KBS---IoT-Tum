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
using System.Threading;

namespace InstallationProgram
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Load += new System.EventHandler(this.Form1_Load);
        }

        Thread CheckComPort;
        Boolean result = true;

        private void ConfigureSensorModule_Click(object sender, EventArgs e)
        {

        }

        public SerialPort comPort = new SerialPort();

        
        
        private void Form1_Load(object sender, EventArgs e)
        {
            CheckComPort = new Thread(new ThreadStart(ComPortCheck));
            CheckComPort.Start();         
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {

            base.OnFormClosing(e);
            CheckComPort.Abort();
        }

        public void ComPortCheck()
        {
            string[] names;
            
            while (true)
            {
                names = SerialPort.GetPortNames();
                
                if (names.Length != 0 && !comPort.IsOpen && result)
                {
                    DialogResult dialogResult = MessageBox.Show(names[0] +" is beschikbaar, wilt u een connectie maken?", "Connectie maken?", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes)
                    {
                        comPort.PortName = names[0];
                        comPort.BaudRate = 115200;
                        comPort.Handshake = 0;
                        comPort.Open();
                        comPort.RtsEnable = true;
                        comPort.DtrEnable = true;
                        comPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

                        askDeviceType();
                    }
                    else if (dialogResult == DialogResult.No)
                    {
                        if (InvokeRequired)
                        {
                            result = false;
                            // after we've done all the processing, 
                            this.Invoke(new MethodInvoker(delegate
                            {
                                //title is changed to no device.
                                setNoDevice();
                                //set sensor module info on false.
                                setSensorInfo(sensorModule.Visible);
                                //set master module info on false.
                                setMasterInfo(masterModule.Visible);
                                if (comPort.IsOpen)
                                {
                                    comPort.Close();
                                }
                            })); 
                        }
                    }
                }
                else if(names.Length == 0)
                {
                    if (InvokeRequired)
                    {
                        // after we've done all the processing, 
                        this.Invoke(new MethodInvoker(delegate
                        {
                            //title is changed to no device.
                            setNoDevice();
                            //set sensor module info on false.
                            setSensorInfo(sensorModule.Visible);
                            //set master module info on false.
                            setMasterInfo(masterModule.Visible);
                            if (comPort.IsOpen)
                            {
                                comPort.Close();
                            }
                        }));
                        
                    }
                }
                Thread.Sleep(1000);
            }
            
        }

        private void setNoDevice()
        {
            noDevice.Visible = true;
            sensorModule.Visible = false;
            masterModule.Visible = false;
        }

        private void setSensorModule()
        {
            sensorModule.Visible = true;
            masterModule.Visible = false;
            noDevice.Visible = false;
        }

        private void setMasterModule()
        {
            sensorModule.Visible = false;
            masterModule.Visible = true;
            noDevice.Visible = false;
        }

        private void askDeviceType()
        {
            //first 1 test character
            comPort.Write("0");

            //ask type of device
            comPort.Write("d");

        }

        private void setSensorInfo(Boolean visible)
        {
            if(visible == true)
            {
                sensorID.Visible = true;
                SensorIDText.Visible = true;
                masterID.Visible = true;
                MasterIDText.Visible = true;
                sensorDistance.Visible = true;
                sensorDistanceText.Visible = true;
                ConfigureSensorModule.Visible = true;
                batteryLevel.Visible = true;
                askBatteryLevel.Visible = true;
                progressBattery.Visible = true;
                NameBluetooth.Visible = true;
                nameBluetoothText.Visible = true;
            }
            else
            {
                sensorID.Visible = false;
                SensorIDText.Visible = false;
                masterID.Visible = false;
                MasterIDText.Visible = false;
                sensorDistance.Visible = false;
                sensorDistanceText.Visible = false;
                ConfigureSensorModule.Visible = false;
                batteryLevel.Visible = false;
                askBatteryLevel.Visible = false;
                progressBattery.Visible = false;
                selectWifi.Visible = false;
                if (noDevice.Visible)
                {
                    NameBluetooth.Visible = false;
                    nameBluetoothText.Visible = false;
                    passwordBluetooth.Visible = false;
                    passwordBluetoothText.Visible = false;
                }
                else
                {
                    NameBluetooth.Visible = true;
                    nameBluetoothText.Visible = true;
                    passwordBluetooth.Visible = true;
                    passwordBluetoothText.Visible = true;
                }
            }
        }

        private void setMasterInfo(Boolean visible)
        {
            if (visible == true)
            {
                masterModuleID.Visible = true;
                MastermoduleIDText.Visible = true;
                chamberID.Visible = true;
                chamberIDText.Visible = true;
                nameID.Visible = true;
                NameIDText.Visible = true;
                ConfigureMasterModule.Visible = true;
                NameBluetooth.Visible = true;
                nameBluetoothText.Visible = true;
                selectWifi.Visible = true;

            }
            else
            {
                masterModuleID.Visible = false;
                MastermoduleIDText.Visible = false;
                chamberID.Visible = false;
                chamberIDText.Visible = false;
                nameID.Visible = false;
                NameIDText.Visible = false;
                ConfigureMasterModule.Visible = false;
                selectWifi.Visible = false;
                if (noDevice.Visible)
                {
                    NameBluetooth.Visible = false;
                    nameBluetoothText.Visible = false;
                    passwordBluetooth.Visible = false;
                    passwordBluetoothText.Visible = false;
                }
                else
                {
                    NameBluetooth.Visible = true;
                    nameBluetoothText.Visible = true;
                    passwordBluetooth.Visible = true;
                    passwordBluetoothText.Visible = true;
                }
            }
        }

        private void ComPortButton_Click(object sender, EventArgs e)
        {
            //if you pressed accidentally no you can connect again with the com port.
            comPort.Close();
            result = true;
        }

        private void DataReceivedHandler(
                         object sender,
                         SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();
            //sensor module
            if(indata == "1"){

                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate {
                        //title is changed to sensor module
                        setSensorModule();

                        //set everything visible of sensor module
                        setSensorInfo(sensorModule.Visible);

                        //everything of master module has to be set on invisible
                        setMasterInfo(masterModule.Visible);
                    }));
                    return;
                }
                
                
            }else if(indata == "2"){         //master module

                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate {
                        //title is changed to mastermodule
                        setMasterModule();

                        //set everything visible for master module
                        setMasterInfo(masterModule.Visible);

                        //set everything invisible for sensor module.
                        setSensorInfo(sensorModule.Visible);
                    }));
                    return;
                }               
            }
        }

        
    }
}
