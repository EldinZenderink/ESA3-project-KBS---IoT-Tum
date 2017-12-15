namespace InstallationProgram
{
    partial class Form1
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
            this.sensorModule = new System.Windows.Forms.Label();
            this.masterModule = new System.Windows.Forms.Label();
            this.noDevice = new System.Windows.Forms.Label();
            this.sensorID = new System.Windows.Forms.Label();
            this.masterID = new System.Windows.Forms.Label();
            this.sensorDistance = new System.Windows.Forms.Label();
            this.batteryLevel = new System.Windows.Forms.Label();
            this.progressBattery = new System.Windows.Forms.ProgressBar();
            this.askBatteryLevel = new System.Windows.Forms.Button();
            this.ConfigureSensorModule = new System.Windows.Forms.Button();
            this.SensorIDText = new System.Windows.Forms.TextBox();
            this.MasterIDText = new System.Windows.Forms.TextBox();
            this.sensorDistanceText = new System.Windows.Forms.TextBox();
            this.masterModuleID = new System.Windows.Forms.Label();
            this.chamberID = new System.Windows.Forms.Label();
            this.nameID = new System.Windows.Forms.Label();
            this.MastermoduleIDText = new System.Windows.Forms.TextBox();
            this.chamberIDText = new System.Windows.Forms.TextBox();
            this.NameIDText = new System.Windows.Forms.TextBox();
            this.ConfigureMasterModule = new System.Windows.Forms.Button();
            this.NameBluetooth = new System.Windows.Forms.Label();
            this.passwordBluetooth = new System.Windows.Forms.Label();
            this.nameBluetoothText = new System.Windows.Forms.TextBox();
            this.passwordBluetoothText = new System.Windows.Forms.TextBox();
            this.selectWifi = new System.Windows.Forms.Button();
            this.ComPortButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // sensorModule
            // 
            this.sensorModule.AutoSize = true;
            this.sensorModule.Font = new System.Drawing.Font("Microsoft Sans Serif", 30F);
            this.sensorModule.Location = new System.Drawing.Point(13, 13);
            this.sensorModule.Name = "sensorModule";
            this.sensorModule.Size = new System.Drawing.Size(278, 46);
            this.sensorModule.TabIndex = 0;
            this.sensorModule.Text = "Sensormodule";
            this.sensorModule.Visible = false;
            // 
            // masterModule
            // 
            this.masterModule.AutoSize = true;
            this.masterModule.Font = new System.Drawing.Font("Microsoft Sans Serif", 30F);
            this.masterModule.Location = new System.Drawing.Point(21, 13);
            this.masterModule.Name = "masterModule";
            this.masterModule.Size = new System.Drawing.Size(272, 46);
            this.masterModule.TabIndex = 1;
            this.masterModule.Text = "Mastermodule";
            this.masterModule.Visible = false;
            // 
            // noDevice
            // 
            this.noDevice.AutoSize = true;
            this.noDevice.Font = new System.Drawing.Font("Microsoft Sans Serif", 30F);
            this.noDevice.Location = new System.Drawing.Point(21, 13);
            this.noDevice.Name = "noDevice";
            this.noDevice.Size = new System.Drawing.Size(487, 46);
            this.noDevice.TabIndex = 2;
            this.noDevice.Text = "Geen module aangesloten";
            this.noDevice.Visible = false;
            // 
            // sensorID
            // 
            this.sensorID.AutoSize = true;
            this.sensorID.Location = new System.Drawing.Point(21, 102);
            this.sensorID.Name = "sensorID";
            this.sensorID.Size = new System.Drawing.Size(54, 13);
            this.sensorID.TabIndex = 3;
            this.sensorID.Text = "Sensor ID";
            this.sensorID.Visible = false;
            // 
            // masterID
            // 
            this.masterID.AutoSize = true;
            this.masterID.Location = new System.Drawing.Point(21, 143);
            this.masterID.Name = "masterID";
            this.masterID.Size = new System.Drawing.Size(53, 13);
            this.masterID.TabIndex = 4;
            this.masterID.Text = "Master ID";
            this.masterID.Visible = false;
            // 
            // sensorDistance
            // 
            this.sensorDistance.AutoSize = true;
            this.sensorDistance.Location = new System.Drawing.Point(21, 182);
            this.sensorDistance.Name = "sensorDistance";
            this.sensorDistance.Size = new System.Drawing.Size(98, 13);
            this.sensorDistance.TabIndex = 5;
            this.sensorDistance.Text = "Afstand van sensor";
            this.sensorDistance.Visible = false;
            // 
            // batteryLevel
            // 
            this.batteryLevel.AutoSize = true;
            this.batteryLevel.Location = new System.Drawing.Point(231, 101);
            this.batteryLevel.Name = "batteryLevel";
            this.batteryLevel.Size = new System.Drawing.Size(74, 13);
            this.batteryLevel.TabIndex = 6;
            this.batteryLevel.Text = "Batterij niveau";
            this.batteryLevel.Visible = false;
            // 
            // progressBattery
            // 
            this.progressBattery.Location = new System.Drawing.Point(312, 102);
            this.progressBattery.Name = "progressBattery";
            this.progressBattery.Size = new System.Drawing.Size(83, 12);
            this.progressBattery.TabIndex = 7;
            this.progressBattery.Visible = false;
            // 
            // askBatteryLevel
            // 
            this.askBatteryLevel.Location = new System.Drawing.Point(234, 132);
            this.askBatteryLevel.Name = "askBatteryLevel";
            this.askBatteryLevel.Size = new System.Drawing.Size(136, 23);
            this.askBatteryLevel.TabIndex = 8;
            this.askBatteryLevel.Text = "Vraag batterij niveau op";
            this.askBatteryLevel.UseVisualStyleBackColor = true;
            this.askBatteryLevel.Visible = false;
            // 
            // ConfigureSensorModule
            // 
            this.ConfigureSensorModule.Location = new System.Drawing.Point(24, 263);
            this.ConfigureSensorModule.Name = "ConfigureSensorModule";
            this.ConfigureSensorModule.Size = new System.Drawing.Size(165, 23);
            this.ConfigureSensorModule.TabIndex = 9;
            this.ConfigureSensorModule.Text = "Configureer sensor module";
            this.ConfigureSensorModule.UseVisualStyleBackColor = true;
            this.ConfigureSensorModule.Visible = false;
            this.ConfigureSensorModule.Click += new System.EventHandler(this.ConfigureSensorModule_Click);
            // 
            // SensorIDText
            // 
            this.SensorIDText.Location = new System.Drawing.Point(160, 95);
            this.SensorIDText.Name = "SensorIDText";
            this.SensorIDText.Size = new System.Drawing.Size(29, 20);
            this.SensorIDText.TabIndex = 10;
            this.SensorIDText.Visible = false;
            // 
            // MasterIDText
            // 
            this.MasterIDText.Location = new System.Drawing.Point(160, 136);
            this.MasterIDText.Name = "MasterIDText";
            this.MasterIDText.Size = new System.Drawing.Size(29, 20);
            this.MasterIDText.TabIndex = 11;
            this.MasterIDText.Visible = false;
            // 
            // sensorDistanceText
            // 
            this.sensorDistanceText.Location = new System.Drawing.Point(160, 174);
            this.sensorDistanceText.Name = "sensorDistanceText";
            this.sensorDistanceText.Size = new System.Drawing.Size(29, 20);
            this.sensorDistanceText.TabIndex = 12;
            this.sensorDistanceText.Visible = false;
            // 
            // masterModuleID
            // 
            this.masterModuleID.AutoSize = true;
            this.masterModuleID.Location = new System.Drawing.Point(24, 101);
            this.masterModuleID.Name = "masterModuleID";
            this.masterModuleID.Size = new System.Drawing.Size(50, 13);
            this.masterModuleID.TabIndex = 13;
            this.masterModuleID.Text = "MasterID";
            this.masterModuleID.Visible = false;
            // 
            // chamberID
            // 
            this.chamberID.AutoSize = true;
            this.chamberID.Location = new System.Drawing.Point(24, 143);
            this.chamberID.Name = "chamberID";
            this.chamberID.Size = new System.Drawing.Size(39, 13);
            this.chamberID.TabIndex = 14;
            this.chamberID.Text = "Lokaal";
            this.chamberID.Visible = false;
            // 
            // nameID
            // 
            this.nameID.AutoSize = true;
            this.nameID.Location = new System.Drawing.Point(24, 182);
            this.nameID.Name = "nameID";
            this.nameID.Size = new System.Drawing.Size(35, 13);
            this.nameID.TabIndex = 15;
            this.nameID.Text = "Naam";
            this.nameID.Visible = false;
            // 
            // MastermoduleIDText
            // 
            this.MastermoduleIDText.Location = new System.Drawing.Point(160, 95);
            this.MastermoduleIDText.Name = "MastermoduleIDText";
            this.MastermoduleIDText.Size = new System.Drawing.Size(29, 20);
            this.MastermoduleIDText.TabIndex = 16;
            this.MastermoduleIDText.Visible = false;
            // 
            // chamberIDText
            // 
            this.chamberIDText.Location = new System.Drawing.Point(160, 136);
            this.chamberIDText.Name = "chamberIDText";
            this.chamberIDText.Size = new System.Drawing.Size(29, 20);
            this.chamberIDText.TabIndex = 17;
            this.chamberIDText.Visible = false;
            // 
            // NameIDText
            // 
            this.NameIDText.Location = new System.Drawing.Point(160, 174);
            this.NameIDText.Name = "NameIDText";
            this.NameIDText.Size = new System.Drawing.Size(100, 20);
            this.NameIDText.TabIndex = 18;
            this.NameIDText.Visible = false;
            // 
            // ConfigureMasterModule
            // 
            this.ConfigureMasterModule.Location = new System.Drawing.Point(24, 263);
            this.ConfigureMasterModule.Name = "ConfigureMasterModule";
            this.ConfigureMasterModule.Size = new System.Drawing.Size(165, 23);
            this.ConfigureMasterModule.TabIndex = 19;
            this.ConfigureMasterModule.Text = "Configureer mastermodule";
            this.ConfigureMasterModule.UseVisualStyleBackColor = true;
            this.ConfigureMasterModule.Visible = false;
            // 
            // NameBluetooth
            // 
            this.NameBluetooth.AutoSize = true;
            this.NameBluetooth.Location = new System.Drawing.Point(269, 174);
            this.NameBluetooth.Name = "NameBluetooth";
            this.NameBluetooth.Size = new System.Drawing.Size(81, 13);
            this.NameBluetooth.TabIndex = 20;
            this.NameBluetooth.Text = "Bluetooth naam";
            // 
            // passwordBluetooth
            // 
            this.passwordBluetooth.AutoSize = true;
            this.passwordBluetooth.Location = new System.Drawing.Point(269, 201);
            this.passwordBluetooth.Name = "passwordBluetooth";
            this.passwordBluetooth.Size = new System.Drawing.Size(113, 13);
            this.passwordBluetooth.TabIndex = 21;
            this.passwordBluetooth.Text = "Bluetooth wachtwoord";
            // 
            // nameBluetoothText
            // 
            this.nameBluetoothText.Location = new System.Drawing.Point(404, 171);
            this.nameBluetoothText.Name = "nameBluetoothText";
            this.nameBluetoothText.Size = new System.Drawing.Size(100, 20);
            this.nameBluetoothText.TabIndex = 23;
            // 
            // passwordBluetoothText
            // 
            this.passwordBluetoothText.Location = new System.Drawing.Point(404, 201);
            this.passwordBluetoothText.Name = "passwordBluetoothText";
            this.passwordBluetoothText.Size = new System.Drawing.Size(100, 20);
            this.passwordBluetoothText.TabIndex = 24;
            // 
            // selectWifi
            // 
            this.selectWifi.Location = new System.Drawing.Point(27, 224);
            this.selectWifi.Name = "selectWifi";
            this.selectWifi.Size = new System.Drawing.Size(75, 23);
            this.selectWifi.TabIndex = 25;
            this.selectWifi.Text = "Select wifi";
            this.selectWifi.UseVisualStyleBackColor = true;
            // 
            // ComPortButton
            // 
            this.ComPortButton.Location = new System.Drawing.Point(404, 254);
            this.ComPortButton.Name = "ComPortButton";
            this.ComPortButton.Size = new System.Drawing.Size(75, 23);
            this.ComPortButton.TabIndex = 26;
            this.ComPortButton.Text = "Check com port again";
            this.ComPortButton.UseVisualStyleBackColor = true;
            this.ComPortButton.Click += new System.EventHandler(this.ComPortButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(516, 289);
            this.Controls.Add(this.ComPortButton);
            this.Controls.Add(this.selectWifi);
            this.Controls.Add(this.passwordBluetoothText);
            this.Controls.Add(this.nameBluetoothText);
            this.Controls.Add(this.passwordBluetooth);
            this.Controls.Add(this.NameBluetooth);
            this.Controls.Add(this.ConfigureMasterModule);
            this.Controls.Add(this.NameIDText);
            this.Controls.Add(this.chamberIDText);
            this.Controls.Add(this.MastermoduleIDText);
            this.Controls.Add(this.nameID);
            this.Controls.Add(this.chamberID);
            this.Controls.Add(this.masterModuleID);
            this.Controls.Add(this.sensorDistanceText);
            this.Controls.Add(this.MasterIDText);
            this.Controls.Add(this.SensorIDText);
            this.Controls.Add(this.ConfigureSensorModule);
            this.Controls.Add(this.askBatteryLevel);
            this.Controls.Add(this.progressBattery);
            this.Controls.Add(this.batteryLevel);
            this.Controls.Add(this.sensorDistance);
            this.Controls.Add(this.masterID);
            this.Controls.Add(this.sensorID);
            this.Controls.Add(this.noDevice);
            this.Controls.Add(this.masterModule);
            this.Controls.Add(this.sensorModule);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.Label sensorModule;
        public System.Windows.Forms.Label masterModule;
        public System.Windows.Forms.Label noDevice;
        private System.Windows.Forms.Label sensorID;
        private System.Windows.Forms.Label masterID;
        private System.Windows.Forms.Label sensorDistance;
        private System.Windows.Forms.Label batteryLevel;
        private System.Windows.Forms.ProgressBar progressBattery;
        private System.Windows.Forms.Button askBatteryLevel;
        private System.Windows.Forms.Button ConfigureSensorModule;
        private System.Windows.Forms.TextBox SensorIDText;
        private System.Windows.Forms.TextBox MasterIDText;
        private System.Windows.Forms.TextBox sensorDistanceText;
        private System.Windows.Forms.Label masterModuleID;
        private System.Windows.Forms.Label chamberID;
        private System.Windows.Forms.Label nameID;
        private System.Windows.Forms.TextBox MastermoduleIDText;
        private System.Windows.Forms.TextBox chamberIDText;
        private System.Windows.Forms.TextBox NameIDText;
        private System.Windows.Forms.Button ConfigureMasterModule;
        private System.Windows.Forms.Label NameBluetooth;
        private System.Windows.Forms.Label passwordBluetooth;
        private System.Windows.Forms.TextBox nameBluetoothText;
        private System.Windows.Forms.TextBox passwordBluetoothText;
        private System.Windows.Forms.Button selectWifi;
        private System.Windows.Forms.Button ComPortButton;
    }
}

