namespace MagneticRuler
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.gb_SerialSet = new System.Windows.Forms.GroupBox();
            this.SensorID = new System.Windows.Forms.NumericUpDown();
            this.Serial_Address = new System.Windows.Forms.Label();
            this.AutoMatch = new System.Windows.Forms.Button();
            this.btn_SerialOpen = new System.Windows.Forms.Button();
            this.Parity = new System.Windows.Forms.ComboBox();
            this.Serial_Parity = new System.Windows.Forms.Label();
            this.Baud = new System.Windows.Forms.ComboBox();
            this.Serial_Baud = new System.Windows.Forms.Label();
            this.SerialCom = new System.Windows.Forms.ComboBox();
            this.Serial_Com = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.ReadData = new System.Windows.Forms.Button();
            this.timer1_Time = new System.Windows.Forms.Timer(this.components);
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.button_Help = new System.Windows.Forms.Button();
            this.helpProvider1 = new System.Windows.Forms.HelpProvider();
            this.timer_RefreshSerialDelay = new System.Windows.Forms.Timer(this.components);
            this.timer_ReadData = new System.Windows.Forms.Timer(this.components);
            this.bt_WriteCircularCount = new System.Windows.Forms.Button();
            this.WriteSensorID = new System.Windows.Forms.NumericUpDown();
            this.bt_WriteDirection = new System.Windows.Forms.Button();
            this.bt_WriteBaud = new System.Windows.Forms.Button();
            this.bt_WriteParity = new System.Windows.Forms.Button();
            this.Write_Diretion = new System.Windows.Forms.Label();
            this.Write_MagnetNum = new System.Windows.Forms.Label();
            this.Write_Parity = new System.Windows.Forms.Label();
            this.WriteDirection = new System.Windows.Forms.ComboBox();
            this.WriteCircularCount = new System.Windows.Forms.ComboBox();
            this.Write_Baud = new System.Windows.Forms.Label();
            this.bt_WriteSensorID = new System.Windows.Forms.Button();
            this.Write_SensorID = new System.Windows.Forms.Label();
            this.WriteBaud = new System.Windows.Forms.ComboBox();
            this.WriteParity = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.Channal1Pos = new System.Windows.Forms.TextBox();
            this.Channal2Pos = new System.Windows.Forms.TextBox();
            this.timer_TimeOut = new System.Windows.Forms.Timer(this.components);
            this.gb_ParametersModify = new System.Windows.Forms.GroupBox();
            this.Language = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.gb_ReadData = new System.Windows.Forms.GroupBox();
            this.gb_SerialSet.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SensorID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WriteSensorID)).BeginInit();
            this.gb_ParametersModify.SuspendLayout();
            this.gb_ReadData.SuspendLayout();
            this.SuspendLayout();
            // 
            // gb_SerialSet
            // 
            this.gb_SerialSet.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.gb_SerialSet.Controls.Add(this.SensorID);
            this.gb_SerialSet.Controls.Add(this.Serial_Address);
            this.gb_SerialSet.Controls.Add(this.AutoMatch);
            this.gb_SerialSet.Controls.Add(this.btn_SerialOpen);
            this.gb_SerialSet.Controls.Add(this.Parity);
            this.gb_SerialSet.Controls.Add(this.Serial_Parity);
            this.gb_SerialSet.Controls.Add(this.Baud);
            this.gb_SerialSet.Controls.Add(this.Serial_Baud);
            this.gb_SerialSet.Controls.Add(this.SerialCom);
            this.gb_SerialSet.Controls.Add(this.Serial_Com);
            this.gb_SerialSet.Dock = System.Windows.Forms.DockStyle.Top;
            this.gb_SerialSet.Location = new System.Drawing.Point(0, 0);
            this.gb_SerialSet.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gb_SerialSet.Name = "gb_SerialSet";
            this.gb_SerialSet.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gb_SerialSet.Size = new System.Drawing.Size(584, 97);
            this.gb_SerialSet.TabIndex = 1;
            this.gb_SerialSet.TabStop = false;
            this.gb_SerialSet.Text = "Communication Settings";
            // 
            // SensorID
            // 
            this.SensorID.BackColor = System.Drawing.SystemColors.Control;
            this.SensorID.Location = new System.Drawing.Point(77, 25);
            this.SensorID.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.SensorID.Name = "SensorID";
            this.SensorID.Size = new System.Drawing.Size(160, 26);
            this.SensorID.TabIndex = 1;
            this.SensorID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.SensorID.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.SensorID.ValueChanged += new System.EventHandler(this.SensorID_ValueChanged);
            // 
            // Serial_Address
            // 
            this.Serial_Address.AutoSize = true;
            this.Serial_Address.Location = new System.Drawing.Point(12, 30);
            this.Serial_Address.Name = "Serial_Address";
            this.Serial_Address.Size = new System.Drawing.Size(64, 16);
            this.Serial_Address.TabIndex = 11;
            this.Serial_Address.Text = "Address";
            // 
            // AutoMatch
            // 
            this.AutoMatch.BackColor = System.Drawing.Color.White;
            this.AutoMatch.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.AutoMatch.Location = new System.Drawing.Point(489, 25);
            this.AutoMatch.Name = "AutoMatch";
            this.AutoMatch.Size = new System.Drawing.Size(80, 26);
            this.AutoMatch.TabIndex = 5;
            this.AutoMatch.Text = "Match";
            this.AutoMatch.UseVisualStyleBackColor = false;
            this.AutoMatch.Click += new System.EventHandler(this.button1_Click);
            // 
            // btn_SerialOpen
            // 
            this.btn_SerialOpen.BackColor = System.Drawing.Color.White;
            this.btn_SerialOpen.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.btn_SerialOpen.Location = new System.Drawing.Point(489, 57);
            this.btn_SerialOpen.Name = "btn_SerialOpen";
            this.btn_SerialOpen.Size = new System.Drawing.Size(80, 26);
            this.btn_SerialOpen.TabIndex = 6;
            this.btn_SerialOpen.Text = "Open";
            this.btn_SerialOpen.UseVisualStyleBackColor = false;
            this.btn_SerialOpen.Click += new System.EventHandler(this.btn_SerialOpen_Click);
            // 
            // Parity
            // 
            this.Parity.BackColor = System.Drawing.SystemColors.Control;
            this.Parity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.Parity.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Parity.FormattingEnabled = true;
            this.Parity.Items.AddRange(new object[] {
            "NONE",
            "EVEN",
            "ODD"});
            this.Parity.Location = new System.Drawing.Point(310, 58);
            this.Parity.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Parity.Name = "Parity";
            this.Parity.Size = new System.Drawing.Size(160, 24);
            this.Parity.TabIndex = 4;
            this.Parity.SelectedIndexChanged += new System.EventHandler(this.Parity_SelectedIndexChanged);
            // 
            // Serial_Parity
            // 
            this.Serial_Parity.AutoSize = true;
            this.Serial_Parity.Location = new System.Drawing.Point(245, 62);
            this.Serial_Parity.Name = "Serial_Parity";
            this.Serial_Parity.Size = new System.Drawing.Size(56, 16);
            this.Serial_Parity.TabIndex = 4;
            this.Serial_Parity.Text = "Parity";
            // 
            // Baud
            // 
            this.Baud.BackColor = System.Drawing.SystemColors.Control;
            this.Baud.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.Baud.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Baud.FormattingEnabled = true;
            this.Baud.ItemHeight = 16;
            this.Baud.Items.AddRange(new object[] {
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.Baud.Location = new System.Drawing.Point(77, 58);
            this.Baud.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Baud.MaxLength = 9;
            this.Baud.Name = "Baud";
            this.Baud.Size = new System.Drawing.Size(160, 24);
            this.Baud.TabIndex = 3;
            this.Baud.SelectedIndexChanged += new System.EventHandler(this.Baud_SelectedIndexChanged);
            this.Baud.TextChanged += new System.EventHandler(this.Baud_TextChanged);
            // 
            // Serial_Baud
            // 
            this.Serial_Baud.AutoSize = true;
            this.Serial_Baud.Location = new System.Drawing.Point(12, 62);
            this.Serial_Baud.Name = "Serial_Baud";
            this.Serial_Baud.Size = new System.Drawing.Size(48, 16);
            this.Serial_Baud.TabIndex = 2;
            this.Serial_Baud.Text = " Baud";
            // 
            // SerialCom
            // 
            this.SerialCom.BackColor = System.Drawing.SystemColors.Control;
            this.SerialCom.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.SerialCom.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.SerialCom.FormattingEnabled = true;
            this.SerialCom.ItemHeight = 16;
            this.SerialCom.Location = new System.Drawing.Point(310, 26);
            this.SerialCom.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.SerialCom.Name = "SerialCom";
            this.SerialCom.Size = new System.Drawing.Size(160, 24);
            this.SerialCom.TabIndex = 2;
            this.SerialCom.SelectedIndexChanged += new System.EventHandler(this.SerialID_SelectedIndexChanged);
            // 
            // Serial_Com
            // 
            this.Serial_Com.AutoSize = true;
            this.Serial_Com.Location = new System.Drawing.Point(245, 30);
            this.Serial_Com.Name = "Serial_Com";
            this.Serial_Com.Size = new System.Drawing.Size(40, 16);
            this.Serial_Com.TabIndex = 1;
            this.Serial_Com.Text = " COM";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.InitialDirectory = "SenorLibrary";
            // 
            // ReadData
            // 
            this.ReadData.BackColor = System.Drawing.Color.White;
            this.ReadData.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.ReadData.Location = new System.Drawing.Point(489, 41);
            this.ReadData.Name = "ReadData";
            this.ReadData.Size = new System.Drawing.Size(80, 26);
            this.ReadData.TabIndex = 3;
            this.ReadData.Text = "Start";
            this.ReadData.UseVisualStyleBackColor = false;
            this.ReadData.Click += new System.EventHandler(this.ReadData_Click);
            // 
            // timer1_Time
            // 
            this.timer1_Time.Enabled = true;
            this.timer1_Time.Interval = 1000;
            this.timer1_Time.Tick += new System.EventHandler(this.timer1_Time_Tick);
            // 
            // toolTip1
            // 
            this.toolTip1.AutoPopDelay = 5000;
            this.toolTip1.InitialDelay = 50;
            this.toolTip1.ReshowDelay = 100;
            // 
            // serialPort1
            // 
            this.serialPort1.ReceivedBytesThreshold = 5;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // button_Help
            // 
            this.button_Help.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button_Help.BackgroundImage")));
            this.button_Help.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button_Help.Location = new System.Drawing.Point(1103, -1);
            this.button_Help.Name = "button_Help";
            this.button_Help.Size = new System.Drawing.Size(20, 18);
            this.button_Help.TabIndex = 29;
            this.button_Help.UseVisualStyleBackColor = true;
            this.button_Help.Click += new System.EventHandler(this.button_Help_Click);
            // 
            // timer_RefreshSerialDelay
            // 
            this.timer_RefreshSerialDelay.Interval = 20;
            this.timer_RefreshSerialDelay.Tick += new System.EventHandler(this.timer_RefreshSerialDelay_Tick);
            // 
            // timer_ReadData
            // 
            this.timer_ReadData.Tick += new System.EventHandler(this.timer_ReadData_Tick);
            // 
            // bt_WriteCircularCount
            // 
            this.bt_WriteCircularCount.BackColor = System.Drawing.Color.White;
            this.bt_WriteCircularCount.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.bt_WriteCircularCount.Location = new System.Drawing.Point(349, 156);
            this.bt_WriteCircularCount.Name = "bt_WriteCircularCount";
            this.bt_WriteCircularCount.Size = new System.Drawing.Size(80, 26);
            this.bt_WriteCircularCount.TabIndex = 9;
            this.bt_WriteCircularCount.Text = "Modify";
            this.bt_WriteCircularCount.UseVisualStyleBackColor = false;
            this.bt_WriteCircularCount.Click += new System.EventHandler(this.bt_WriteCircularCount_Click);
            // 
            // WriteSensorID
            // 
            this.WriteSensorID.BackColor = System.Drawing.SystemColors.Control;
            this.WriteSensorID.Location = new System.Drawing.Point(236, 36);
            this.WriteSensorID.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.WriteSensorID.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.WriteSensorID.Name = "WriteSensorID";
            this.WriteSensorID.Size = new System.Drawing.Size(90, 26);
            this.WriteSensorID.TabIndex = 1;
            this.WriteSensorID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.WriteSensorID.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.WriteSensorID.ValueChanged += new System.EventHandler(this.WriteSensorID_ValueChanged);
            // 
            // bt_WriteDirection
            // 
            this.bt_WriteDirection.BackColor = System.Drawing.Color.White;
            this.bt_WriteDirection.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.bt_WriteDirection.Location = new System.Drawing.Point(349, 196);
            this.bt_WriteDirection.Name = "bt_WriteDirection";
            this.bt_WriteDirection.Size = new System.Drawing.Size(80, 26);
            this.bt_WriteDirection.TabIndex = 10;
            this.bt_WriteDirection.Text = "Modify";
            this.bt_WriteDirection.UseVisualStyleBackColor = false;
            this.bt_WriteDirection.Click += new System.EventHandler(this.bt_WriteDirection_Click_1);
            // 
            // bt_WriteBaud
            // 
            this.bt_WriteBaud.BackColor = System.Drawing.Color.White;
            this.bt_WriteBaud.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.bt_WriteBaud.Location = new System.Drawing.Point(349, 76);
            this.bt_WriteBaud.Name = "bt_WriteBaud";
            this.bt_WriteBaud.Size = new System.Drawing.Size(80, 26);
            this.bt_WriteBaud.TabIndex = 7;
            this.bt_WriteBaud.Text = "Modify";
            this.bt_WriteBaud.UseVisualStyleBackColor = false;
            this.bt_WriteBaud.Click += new System.EventHandler(this.bt_WriteBaud_Click);
            // 
            // bt_WriteParity
            // 
            this.bt_WriteParity.BackColor = System.Drawing.Color.White;
            this.bt_WriteParity.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.bt_WriteParity.Location = new System.Drawing.Point(349, 116);
            this.bt_WriteParity.Name = "bt_WriteParity";
            this.bt_WriteParity.Size = new System.Drawing.Size(80, 26);
            this.bt_WriteParity.TabIndex = 8;
            this.bt_WriteParity.Text = "Modify";
            this.bt_WriteParity.UseVisualStyleBackColor = false;
            this.bt_WriteParity.Click += new System.EventHandler(this.bt_WriteParity_Click);
            // 
            // Write_Diretion
            // 
            this.Write_Diretion.AutoSize = true;
            this.Write_Diretion.Location = new System.Drawing.Point(151, 201);
            this.Write_Diretion.Name = "Write_Diretion";
            this.Write_Diretion.Size = new System.Drawing.Size(80, 16);
            this.Write_Diretion.TabIndex = 63;
            this.Write_Diretion.Text = "Direction";
            // 
            // Write_MagnetNum
            // 
            this.Write_MagnetNum.AutoSize = true;
            this.Write_MagnetNum.Location = new System.Drawing.Point(151, 161);
            this.Write_MagnetNum.Name = "Write_MagnetNum";
            this.Write_MagnetNum.Size = new System.Drawing.Size(80, 16);
            this.Write_MagnetNum.TabIndex = 62;
            this.Write_MagnetNum.Text = "MagnetNum";
            // 
            // Write_Parity
            // 
            this.Write_Parity.AutoSize = true;
            this.Write_Parity.Location = new System.Drawing.Point(151, 121);
            this.Write_Parity.Name = "Write_Parity";
            this.Write_Parity.Size = new System.Drawing.Size(56, 16);
            this.Write_Parity.TabIndex = 60;
            this.Write_Parity.Text = "Parity";
            // 
            // WriteDirection
            // 
            this.WriteDirection.BackColor = System.Drawing.SystemColors.Control;
            this.WriteDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.WriteDirection.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.WriteDirection.FormattingEnabled = true;
            this.WriteDirection.ItemHeight = 16;
            this.WriteDirection.Items.AddRange(new object[] {
            "Positive",
            "Reverse"});
            this.WriteDirection.Location = new System.Drawing.Point(236, 197);
            this.WriteDirection.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.WriteDirection.MaxLength = 9;
            this.WriteDirection.Name = "WriteDirection";
            this.WriteDirection.Size = new System.Drawing.Size(90, 24);
            this.WriteDirection.TabIndex = 5;
            this.WriteDirection.SelectedIndexChanged += new System.EventHandler(this.WriteDirection_SelectedIndexChanged);
            // 
            // WriteCircularCount
            // 
            this.WriteCircularCount.BackColor = System.Drawing.SystemColors.Control;
            this.WriteCircularCount.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.WriteCircularCount.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.WriteCircularCount.FormattingEnabled = true;
            this.WriteCircularCount.ItemHeight = 16;
            this.WriteCircularCount.Items.AddRange(new object[] {
            "One",
            "Two"});
            this.WriteCircularCount.Location = new System.Drawing.Point(236, 157);
            this.WriteCircularCount.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.WriteCircularCount.MaxLength = 9;
            this.WriteCircularCount.Name = "WriteCircularCount";
            this.WriteCircularCount.Size = new System.Drawing.Size(90, 24);
            this.WriteCircularCount.TabIndex = 4;
            this.WriteCircularCount.SelectedIndexChanged += new System.EventHandler(this.WriteCircularCount_SelectedIndexChanged);
            // 
            // Write_Baud
            // 
            this.Write_Baud.AutoSize = true;
            this.Write_Baud.Location = new System.Drawing.Point(151, 81);
            this.Write_Baud.Name = "Write_Baud";
            this.Write_Baud.Size = new System.Drawing.Size(40, 16);
            this.Write_Baud.TabIndex = 59;
            this.Write_Baud.Text = "Baud";
            // 
            // bt_WriteSensorID
            // 
            this.bt_WriteSensorID.BackColor = System.Drawing.Color.White;
            this.bt_WriteSensorID.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.bt_WriteSensorID.Location = new System.Drawing.Point(349, 36);
            this.bt_WriteSensorID.Name = "bt_WriteSensorID";
            this.bt_WriteSensorID.Size = new System.Drawing.Size(80, 26);
            this.bt_WriteSensorID.TabIndex = 6;
            this.bt_WriteSensorID.Text = "Modify";
            this.bt_WriteSensorID.UseVisualStyleBackColor = false;
            this.bt_WriteSensorID.Click += new System.EventHandler(this.bt_WriteSensorID_Click);
            // 
            // Write_SensorID
            // 
            this.Write_SensorID.AutoSize = true;
            this.Write_SensorID.Location = new System.Drawing.Point(151, 41);
            this.Write_SensorID.Name = "Write_SensorID";
            this.Write_SensorID.Size = new System.Drawing.Size(64, 16);
            this.Write_SensorID.TabIndex = 56;
            this.Write_SensorID.Text = "Address";
            // 
            // WriteBaud
            // 
            this.WriteBaud.BackColor = System.Drawing.SystemColors.Control;
            this.WriteBaud.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.WriteBaud.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.WriteBaud.FormattingEnabled = true;
            this.WriteBaud.ItemHeight = 16;
            this.WriteBaud.Items.AddRange(new object[] {
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.WriteBaud.Location = new System.Drawing.Point(236, 77);
            this.WriteBaud.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.WriteBaud.MaxLength = 9;
            this.WriteBaud.Name = "WriteBaud";
            this.WriteBaud.Size = new System.Drawing.Size(90, 24);
            this.WriteBaud.TabIndex = 2;
            this.WriteBaud.SelectedIndexChanged += new System.EventHandler(this.WriteBaud_SelectedIndexChanged);
            // 
            // WriteParity
            // 
            this.WriteParity.BackColor = System.Drawing.SystemColors.Control;
            this.WriteParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.WriteParity.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.WriteParity.FormattingEnabled = true;
            this.WriteParity.Items.AddRange(new object[] {
            "NONE",
            "EVEN",
            "ODD"});
            this.WriteParity.Location = new System.Drawing.Point(236, 117);
            this.WriteParity.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.WriteParity.Name = "WriteParity";
            this.WriteParity.Size = new System.Drawing.Size(90, 24);
            this.WriteParity.TabIndex = 3;
            this.WriteParity.SelectedIndexChanged += new System.EventHandler(this.WriteParity_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.label6.Font = new System.Drawing.Font("黑体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.ForeColor = System.Drawing.Color.Red;
            this.label6.Location = new System.Drawing.Point(448, 64);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(21, 14);
            this.label6.TabIndex = 59;
            this.label6.Text = "mm";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.label8.Font = new System.Drawing.Font("黑体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.ForeColor = System.Drawing.Color.Red;
            this.label8.Location = new System.Drawing.Point(215, 64);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(21, 14);
            this.label8.TabIndex = 56;
            this.label8.Text = "mm";
            // 
            // Channal1Pos
            // 
            this.Channal1Pos.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.Channal1Pos.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.Channal1Pos.Font = new System.Drawing.Font("黑体", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Channal1Pos.Location = new System.Drawing.Point(57, 29);
            this.Channal1Pos.Name = "Channal1Pos";
            this.Channal1Pos.ReadOnly = true;
            this.Channal1Pos.Size = new System.Drawing.Size(180, 50);
            this.Channal1Pos.TabIndex = 1;
            this.Channal1Pos.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Channal1Pos.MouseDown += new System.Windows.Forms.MouseEventHandler(this.textBox2_MouseDown);
            // 
            // Channal2Pos
            // 
            this.Channal2Pos.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.Channal2Pos.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.Channal2Pos.Font = new System.Drawing.Font("黑体", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Channal2Pos.Location = new System.Drawing.Point(290, 29);
            this.Channal2Pos.Name = "Channal2Pos";
            this.Channal2Pos.ReadOnly = true;
            this.Channal2Pos.Size = new System.Drawing.Size(180, 50);
            this.Channal2Pos.TabIndex = 2;
            this.Channal2Pos.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.Channal2Pos.MouseDown += new System.Windows.Forms.MouseEventHandler(this.textBox1_MouseDown);
            // 
            // timer_TimeOut
            // 
            this.timer_TimeOut.Interval = 200;
            // 
            // gb_ParametersModify
            // 
            this.gb_ParametersModify.Controls.Add(this.bt_WriteSensorID);
            this.gb_ParametersModify.Controls.Add(this.bt_WriteCircularCount);
            this.gb_ParametersModify.Controls.Add(this.Language);
            this.gb_ParametersModify.Controls.Add(this.Write_Baud);
            this.gb_ParametersModify.Controls.Add(this.WriteBaud);
            this.gb_ParametersModify.Controls.Add(this.bt_WriteDirection);
            this.gb_ParametersModify.Controls.Add(this.Write_Parity);
            this.gb_ParametersModify.Controls.Add(this.WriteSensorID);
            this.gb_ParametersModify.Controls.Add(this.WriteParity);
            this.gb_ParametersModify.Controls.Add(this.Write_Diretion);
            this.gb_ParametersModify.Controls.Add(this.Write_SensorID);
            this.gb_ParametersModify.Controls.Add(this.bt_WriteParity);
            this.gb_ParametersModify.Controls.Add(this.Write_MagnetNum);
            this.gb_ParametersModify.Controls.Add(this.WriteCircularCount);
            this.gb_ParametersModify.Controls.Add(this.bt_WriteBaud);
            this.gb_ParametersModify.Controls.Add(this.WriteDirection);
            this.gb_ParametersModify.Dock = System.Windows.Forms.DockStyle.Top;
            this.gb_ParametersModify.Location = new System.Drawing.Point(0, 191);
            this.gb_ParametersModify.Name = "gb_ParametersModify";
            this.gb_ParametersModify.Size = new System.Drawing.Size(584, 248);
            this.gb_ParametersModify.TabIndex = 3;
            this.gb_ParametersModify.TabStop = false;
            this.gb_ParametersModify.Text = "Parameters Settings";
            // 
            // Language
            // 
            this.Language.BackColor = System.Drawing.Color.White;
            this.Language.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.Language.Location = new System.Drawing.Point(537, 212);
            this.Language.Name = "Language";
            this.Language.Size = new System.Drawing.Size(35, 26);
            this.Language.TabIndex = 11;
            this.Language.Text = "CN";
            this.Language.UseVisualStyleBackColor = false;
            this.Language.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("黑体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(23, 46);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 16);
            this.label5.TabIndex = 12;
            this.label5.Text = "CH1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("黑体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(256, 46);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(32, 16);
            this.label7.TabIndex = 71;
            this.label7.Text = "CH2";
            // 
            // gb_ReadData
            // 
            this.gb_ReadData.Controls.Add(this.label6);
            this.gb_ReadData.Controls.Add(this.label8);
            this.gb_ReadData.Controls.Add(this.ReadData);
            this.gb_ReadData.Controls.Add(this.label7);
            this.gb_ReadData.Controls.Add(this.Channal2Pos);
            this.gb_ReadData.Controls.Add(this.label5);
            this.gb_ReadData.Controls.Add(this.Channal1Pos);
            this.gb_ReadData.Dock = System.Windows.Forms.DockStyle.Top;
            this.gb_ReadData.Location = new System.Drawing.Point(0, 97);
            this.gb_ReadData.Name = "gb_ReadData";
            this.gb_ReadData.Size = new System.Drawing.Size(584, 94);
            this.gb_ReadData.TabIndex = 2;
            this.gb_ReadData.TabStop = false;
            this.gb_ReadData.Text = "Read Data";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSize = true;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(584, 441);
            this.Controls.Add(this.gb_ParametersModify);
            this.Controls.Add(this.gb_ReadData);
            this.Controls.Add(this.button_Help);
            this.Controls.Add(this.gb_SerialSet);
            this.Font = new System.Drawing.Font("黑体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.HelpButton = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(600, 480);
            this.MinimumSize = new System.Drawing.Size(600, 480);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Magnetic Ruler";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.gb_SerialSet.ResumeLayout(false);
            this.gb_SerialSet.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SensorID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WriteSensorID)).EndInit();
            this.gb_ParametersModify.ResumeLayout(false);
            this.gb_ParametersModify.PerformLayout();
            this.gb_ReadData.ResumeLayout(false);
            this.gb_ReadData.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gb_SerialSet;
        private System.Windows.Forms.ComboBox Parity;
        private System.Windows.Forms.Label Serial_Parity;
        private System.Windows.Forms.ComboBox Baud;
        private System.Windows.Forms.Label Serial_Baud;
        private System.Windows.Forms.Label Serial_Com;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button ReadData;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button btn_SerialOpen;
        private System.Windows.Forms.Button button_Help;
        private System.Windows.Forms.HelpProvider helpProvider1;
        private System.Windows.Forms.Timer timer_RefreshSerialDelay;
        private System.Windows.Forms.Timer timer1_Time;
        private System.Windows.Forms.Timer timer_ReadData;
        private System.Windows.Forms.Button AutoMatch;
        private System.Windows.Forms.ComboBox WriteCircularCount;
        private System.Windows.Forms.ComboBox WriteDirection;
        private System.Windows.Forms.ComboBox WriteParity;
        private System.Windows.Forms.ComboBox WriteBaud;
        private System.Windows.Forms.NumericUpDown SensorID;
        private System.Windows.Forms.Label Serial_Address;
        private System.Windows.Forms.ComboBox SerialCom;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button bt_WriteSensorID;
        private System.Windows.Forms.Label Write_SensorID;
        private System.Windows.Forms.Label Write_Diretion;
        private System.Windows.Forms.Label Write_MagnetNum;
        private System.Windows.Forms.Label Write_Parity;
        private System.Windows.Forms.Label Write_Baud;
        private System.Windows.Forms.Button bt_WriteCircularCount;
        private System.Windows.Forms.Button bt_WriteDirection;
        private System.Windows.Forms.Button bt_WriteBaud;
        private System.Windows.Forms.Button bt_WriteParity;
        private System.Windows.Forms.NumericUpDown WriteSensorID;
        private System.Windows.Forms.Timer timer_TimeOut;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox Channal1Pos;
        private System.Windows.Forms.TextBox Channal2Pos;
        private System.Windows.Forms.GroupBox gb_ParametersModify;
        private System.Windows.Forms.Button Language;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox gb_ReadData;
    }
}

