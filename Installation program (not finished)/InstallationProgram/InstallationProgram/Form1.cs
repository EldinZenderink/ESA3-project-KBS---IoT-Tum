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
        Boolean receiveOriginalData = false;

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
            if (comPort.IsOpen)
            {
                comPort.Close();
            }
            CheckComPort.Abort();
        }

        public void ComPortCheck()
        {
            string[] names;

            while (true)
            {
                names = SerialPort.GetPortNames();

                if (comPort.IsOpen)
                {
                    askDeviceType();
                }

                if (names.Length != 0 && !comPort.IsOpen && result)
                {
                    DialogResult dialogResult = MessageBox.Show(names[0] + " is beschikbaar, wilt u een connectie maken?", "Connectie maken?", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes)
                    {
                        comPort.PortName = names[0];
                        comPort.BaudRate = 115200;
                        comPort.Handshake = 0;
                        comPort.Open();
                        comPort.RtsEnable = true;
                        comPort.DtrEnable = true;
                        comPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

                        comPort.Write("d");

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
                else if (names.Length == 0)
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
            //comPort.Write("0");

            //ask type of device
            comPort.Write("d");

        }

        private void setSensorInfo(Boolean visible)
        {
            if (visible == true)
            {
                sensorID.Visible = true;
                SensorIDText.Visible = true;
                masterID.Visible = true;
                MasterIDText.Visible = true;
                sensorDistance.Visible = true;
                sensorDistanceText.Visible = true;
                ConfigureSensorModule.Visible = true;
                batteryLevel.Visible = true;
                progressBattery.Visible = true;
                NameBluetooth.Visible = true;
                nameBluetoothText.Visible = true;
                askForData.Visible = true;
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
                progressBattery.Visible = false;
                selectWifi.Visible = false;
                if (noDevice.Visible)
                {
                    NameBluetooth.Visible = false;
                    nameBluetoothText.Visible = false;
                    passwordBluetooth.Visible = false;
                    passwordBluetoothText.Visible = false;
                    askForData.Visible = false;
                }
                else
                {
                    NameBluetooth.Visible = true;
                    nameBluetoothText.Visible = true;
                    passwordBluetooth.Visible = true;
                    passwordBluetoothText.Visible = true;
                    askForData.Visible = true;
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
                askForData.Visible = true;

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
                    askForData.Visible = false;
                }
                else
                {
                    NameBluetooth.Visible = true;
                    nameBluetoothText.Visible = true;
                    passwordBluetooth.Visible = true;
                    passwordBluetoothText.Visible = true;
                    askForData.Visible = true;
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
            Boolean sensorIDOriginal = false;
            Boolean masterIDOriginal = false;
            Boolean distanceSensorOriginal = false;
            Boolean batteryLevelOriginal = false;
            int amountOfDecimals = 0;
            int?[] array2 = new int?[4];
            //sensor module
            if (indata == "1" && !sensorModule.Visible && !masterModule.Visible)
            {

                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
                        //title is changed to sensor module
                        setSensorModule();
                        //set everything visible of sensor module
                        setSensorInfo(sensorModule.Visible);
                        //everything of master module has to be set on invisible
                        setMasterInfo(masterModule.Visible);
                        //start transmission
                        comPort.Write("s");
                        receiveOriginalData = true;
                    }));
                    //return;
                }
            }
            else if (indata == "2" && !masterModule.Visible && !sensorModule.Visible)   //master module
            {         
                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
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
            // Dit werkt nog niet indata 1 t/m 0 en is ook niet heel mooi.
            /*
            else if (sensorModule.Visible)
            {

                if (indata == "1")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 1;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "2")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 2;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "3")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 3;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "4")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 4;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "5")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 5;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "6")
                {

                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 6;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "7")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 7;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "8")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 8;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "9")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 9;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                else if (indata == "0")
                {
                    for (int i = 0; i < array2.Length + 1; i++)
                    {
                        if (array2[i] == null)
                        {
                            array2[i] = 0;
                            amountOfDecimals++;
                            break;
                        }
                    }
                    
                }
                */
            else if (indata == "a")
            {
                sensorIDOriginal = true;
            }
            else if (indata == "b")
            {
                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
                            //SensorIDText.Text = parseInteger(amountOfDecimals, array2).ToString();
                            SensorIDText.Text = "19";       //testing
                            amountOfDecimals = 0;
                        masterIDOriginal = true;
                    }));
                }
            }
            else if (indata == "c")
            {
                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
                            //MasterIDText.Text = parseInteger(amountOfDecimals, array2).ToString();
                            MasterIDText.Text = "16";       //testing
                            amountOfDecimals = 0;
                        distanceSensorOriginal = true;
                    }));
                }
            }
            else if (indata == "d")
            {
                if (InvokeRequired)
                {
                    // after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
                            //sensorDistanceText.Text = parseInteger(amountOfDecimals, array2).ToString();
                            sensorDistanceText.Text = "49";     //testing
                            amountOfDecimals = 0;
                        batteryLevelOriginal = true;
                    }));
                }
            }
            else if (indata == "q")
            {
                if (InvokeRequired)
                {
                    //after we've done all the processing, 
                    this.Invoke(new MethodInvoker(delegate
                    {
                            //progressBattery.Value = (int)parseInteger(amountOfDecimals, array2);
                            progressBattery.Value = 55;       //testing
                            amountOfDecimals = 0;
                        receiveOriginalData = false;
                    }));
                }
            }



            
        }
            
        }
    /*
        //Array contains values 0-9 per index, combines values in array to single integer, depending how many single decimals(0-9) are in the array
        int? parseInteger(int amountOfDecimals, int?[] array)
        {
            //hello
            int? total;
            if (amountOfDecimals == 1)
            {
                total = array[0];
            }
            else if (amountOfDecimals == 2)
            {
                total = (array[0] * 10 + array[1]);
            }
            else if (amountOfDecimals == 3)
            {
                total = (array[0] * 100 + array[1] * 10 + array[2]);
            }
            else if (amountOfDecimals == 4)
            {
                total = (array[0] * 1000 + array[1] * 100 + array[2] * 10 + array[3]);
            }
            else
            {
                total = 0;
            }
            return total;
        }

        private void askForData_Click(object sender, EventArgs e)
        {
            comPort.Write("s");
        }
    }
    */
}
