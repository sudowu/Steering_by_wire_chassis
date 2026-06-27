namespace control_software
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.timer_rec = new System.Windows.Forms.Timer(this.components);
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.panelStatusBar = new System.Windows.Forms.Panel();
            this.labelStatusConn = new System.Windows.Forms.Label();
            this.labelStatusCAN = new System.Windows.Forms.Label();
            this.labelStatusDev = new System.Windows.Forms.Label();
            this.labelStatusCh = new System.Windows.Forms.Label();
            this.groupBox1 = new control_software.Controls.ModernGroupBox();
            this.comboBox_devtype = new control_software.Controls.ModernComboBox();
            this.comboBox_DevIndex = new control_software.Controls.ModernComboBox();
            this.comboBox_CANIndex = new control_software.Controls.ModernComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new control_software.Controls.ModernGroupBox();
            this.textBox_AccCode = new System.Windows.Forms.TextBox();
            this.textBox_AccMask = new System.Windows.Forms.TextBox();
            this.textBox_Time0 = new System.Windows.Forms.TextBox();
            this.textBox_Time1 = new System.Windows.Forms.TextBox();
            this.comboBox_Filter = new control_software.Controls.ModernComboBox();
            this.comboBox_Mode = new control_software.Controls.ModernComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.buttonConnect = new control_software.Controls.ModernButton();
            this.button_StartCAN = new control_software.Controls.ModernButton();
            this.button_StopCAN = new control_software.Controls.ModernButton();
            this.groupBox4 = new control_software.Controls.ModernGroupBox();
            this.button_advance = new control_software.Controls.ModernButton();
            this.button_Retreat = new control_software.Controls.ModernButton();
            this.button_Stop = new control_software.Controls.ModernButton();
            this.button_turn_left = new control_software.Controls.ModernButton();
            this.button_turn_right = new control_software.Controls.ModernButton();
            this.trackBar_Power = new System.Windows.Forms.TrackBar();
            this.trackBar_direction = new System.Windows.Forms.TrackBar();
            this.label_power = new System.Windows.Forms.Label();
            this.label_direction = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.groupBox3 = new control_software.Controls.ModernGroupBox();
            this.textBox_motor1 = new System.Windows.Forms.TextBox();
            this.textBox_motor2 = new System.Windows.Forms.TextBox();
            this.button_brake_state1 = new control_software.Controls.ModernButton();
            this.button_brake_state2 = new control_software.Controls.ModernButton();
            this.textBox_Remote2 = new System.Windows.Forms.TextBox();
            this.textBox_Remote4 = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.groupBox5 = new control_software.Controls.ModernGroupBox();
            this.comboBox_FrameFormat = new control_software.Controls.ModernComboBox();
            this.comboBox_FrameType = new control_software.Controls.ModernComboBox();
            this.textBox_ID = new System.Windows.Forms.TextBox();
            this.textBox_Data = new System.Windows.Forms.TextBox();
            this.button_Send = new control_software.Controls.ModernButton();
            this.button_Clear = new control_software.Controls.ModernButton();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.groupBox6 = new control_software.Controls.ModernGroupBox();
            this.listView_Info = new System.Windows.Forms.ListView();
            this.panelStatusBar.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Power)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_direction)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // timer_rec
            // 
            this.timer_rec.Tick += new System.EventHandler(this.timer_rec_Tick);
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "red_light.png");
            this.imageList1.Images.SetKeyName(1, "green_light.png");
            // 
            // panelStatusBar
            // 
            this.panelStatusBar.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(25)))), ((int)(((byte)(38)))));
            this.panelStatusBar.Controls.Add(this.labelStatusConn);
            this.panelStatusBar.Controls.Add(this.labelStatusCAN);
            this.panelStatusBar.Controls.Add(this.labelStatusDev);
            this.panelStatusBar.Controls.Add(this.labelStatusCh);
            this.panelStatusBar.Location = new System.Drawing.Point(0, 0);
            this.panelStatusBar.Name = "panelStatusBar";
            this.panelStatusBar.Size = new System.Drawing.Size(1360, 44);
            this.panelStatusBar.TabIndex = 0;
            // 
            // labelStatusConn
            // 
            this.labelStatusConn.AutoSize = true;
            this.labelStatusConn.Font = new System.Drawing.Font("Segoe UI", 10F, System.Drawing.FontStyle.Bold);
            this.labelStatusConn.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(104)))), ((int)(((byte)(128)))));
            this.labelStatusConn.Location = new System.Drawing.Point(20, 12);
            this.labelStatusConn.Name = "labelStatusConn";
            this.labelStatusConn.Size = new System.Drawing.Size(99, 28);
            this.labelStatusConn.TabIndex = 0;
            this.labelStatusConn.Text = "●  未连接";
            // 
            // labelStatusCAN
            // 
            this.labelStatusCAN.AutoSize = true;
            this.labelStatusCAN.Font = new System.Drawing.Font("Segoe UI", 10F, System.Drawing.FontStyle.Bold);
            this.labelStatusCAN.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(104)))), ((int)(((byte)(128)))));
            this.labelStatusCAN.Location = new System.Drawing.Point(210, 12);
            this.labelStatusCAN.Name = "labelStatusCAN";
            this.labelStatusCAN.Size = new System.Drawing.Size(102, 28);
            this.labelStatusCAN.TabIndex = 1;
            this.labelStatusCAN.Text = "CAN 停止";
            // 
            // labelStatusDev
            // 
            this.labelStatusDev.AutoSize = true;
            this.labelStatusDev.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.labelStatusDev.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(139)))), ((int)(((byte)(155)))), ((int)(((byte)(180)))));
            this.labelStatusDev.Location = new System.Drawing.Point(500, 12);
            this.labelStatusDev.Name = "labelStatusDev";
            this.labelStatusDev.Size = new System.Drawing.Size(32, 28);
            this.labelStatusDev.TabIndex = 2;
            this.labelStatusDev.Text = "—";
            // 
            // labelStatusCh
            // 
            this.labelStatusCh.AutoSize = true;
            this.labelStatusCh.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.labelStatusCh.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(139)))), ((int)(((byte)(155)))), ((int)(((byte)(180)))));
            this.labelStatusCh.Location = new System.Drawing.Point(650, 12);
            this.labelStatusCh.Name = "labelStatusCh";
            this.labelStatusCh.Size = new System.Drawing.Size(77, 28);
            this.labelStatusCh.TabIndex = 3;
            this.labelStatusCh.Text = "通道 —";
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox1.Controls.Add(this.comboBox_devtype);
            this.groupBox1.Controls.Add(this.comboBox_DevIndex);
            this.groupBox1.Controls.Add(this.comboBox_CANIndex);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.buttonConnect);
            this.groupBox1.Controls.Add(this.button_StartCAN);
            this.groupBox1.Controls.Add(this.button_StopCAN);
            this.groupBox1.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox1.Location = new System.Drawing.Point(12, 56);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(308, 514);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "设备配置";
            // 
            // comboBox_devtype
            // 
            this.comboBox_devtype.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_devtype.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_devtype.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_devtype.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_devtype.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_devtype.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_devtype.ItemHeight = 26;
            this.comboBox_devtype.Location = new System.Drawing.Point(126, 32);
            this.comboBox_devtype.Name = "comboBox_devtype";
            this.comboBox_devtype.Size = new System.Drawing.Size(158, 32);
            this.comboBox_devtype.TabIndex = 1;
            // 
            // comboBox_DevIndex
            // 
            this.comboBox_DevIndex.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_DevIndex.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_DevIndex.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_DevIndex.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_DevIndex.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_DevIndex.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_DevIndex.ItemHeight = 26;
            this.comboBox_DevIndex.Location = new System.Drawing.Point(126, 70);
            this.comboBox_DevIndex.Name = "comboBox_DevIndex";
            this.comboBox_DevIndex.Size = new System.Drawing.Size(158, 32);
            this.comboBox_DevIndex.TabIndex = 2;
            // 
            // comboBox_CANIndex
            // 
            this.comboBox_CANIndex.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_CANIndex.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_CANIndex.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_CANIndex.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_CANIndex.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_CANIndex.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_CANIndex.ItemHeight = 26;
            this.comboBox_CANIndex.Location = new System.Drawing.Point(126, 108);
            this.comboBox_CANIndex.Name = "comboBox_CANIndex";
            this.comboBox_CANIndex.Size = new System.Drawing.Size(158, 32);
            this.comboBox_CANIndex.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(92, 28);
            this.label1.TabIndex = 4;
            this.label1.Text = "设备类型";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 74);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 28);
            this.label2.TabIndex = 5;
            this.label2.Text = "设备索引";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 112);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(97, 28);
            this.label3.TabIndex = 6;
            this.label3.Text = "CAN 通道";
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox2.Controls.Add(this.textBox_AccCode);
            this.groupBox2.Controls.Add(this.textBox_AccMask);
            this.groupBox2.Controls.Add(this.textBox_Time0);
            this.groupBox2.Controls.Add(this.textBox_Time1);
            this.groupBox2.Controls.Add(this.comboBox_Filter);
            this.groupBox2.Controls.Add(this.comboBox_Mode);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox2.Location = new System.Drawing.Point(8, 150);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(292, 270);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "CAN 参数";
            // 
            // textBox_AccCode
            // 
            this.textBox_AccCode.Location = new System.Drawing.Point(116, 30);
            this.textBox_AccCode.Name = "textBox_AccCode";
            this.textBox_AccCode.Size = new System.Drawing.Size(158, 34);
            this.textBox_AccCode.TabIndex = 1;
            // 
            // textBox_AccMask
            // 
            this.textBox_AccMask.Location = new System.Drawing.Point(116, 66);
            this.textBox_AccMask.Name = "textBox_AccMask";
            this.textBox_AccMask.Size = new System.Drawing.Size(158, 34);
            this.textBox_AccMask.TabIndex = 2;
            // 
            // textBox_Time0
            // 
            this.textBox_Time0.Location = new System.Drawing.Point(116, 102);
            this.textBox_Time0.Name = "textBox_Time0";
            this.textBox_Time0.Size = new System.Drawing.Size(158, 34);
            this.textBox_Time0.TabIndex = 3;
            // 
            // textBox_Time1
            // 
            this.textBox_Time1.Location = new System.Drawing.Point(116, 138);
            this.textBox_Time1.Name = "textBox_Time1";
            this.textBox_Time1.Size = new System.Drawing.Size(158, 34);
            this.textBox_Time1.TabIndex = 4;
            // 
            // comboBox_Filter
            // 
            this.comboBox_Filter.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_Filter.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_Filter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Filter.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_Filter.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_Filter.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_Filter.ItemHeight = 26;
            this.comboBox_Filter.Location = new System.Drawing.Point(116, 174);
            this.comboBox_Filter.Name = "comboBox_Filter";
            this.comboBox_Filter.Size = new System.Drawing.Size(158, 32);
            this.comboBox_Filter.TabIndex = 5;
            // 
            // comboBox_Mode
            // 
            this.comboBox_Mode.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_Mode.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_Mode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Mode.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_Mode.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_Mode.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_Mode.ItemHeight = 26;
            this.comboBox_Mode.Location = new System.Drawing.Point(116, 210);
            this.comboBox_Mode.Name = "comboBox_Mode";
            this.comboBox_Mode.Size = new System.Drawing.Size(158, 32);
            this.comboBox_Mode.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 34);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 28);
            this.label4.TabIndex = 7;
            this.label4.Text = "验收码";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 70);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(72, 28);
            this.label5.TabIndex = 8;
            this.label5.Text = "屏蔽码";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 106);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(88, 28);
            this.label6.TabIndex = 9;
            this.label6.Text = "定时器 0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(12, 142);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(88, 28);
            this.label7.TabIndex = 10;
            this.label7.Text = "定时器 1";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 214);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(92, 28);
            this.label8.TabIndex = 11;
            this.label8.Text = "工作模式";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 178);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(92, 28);
            this.label9.TabIndex = 12;
            this.label9.Text = "滤波方式";
            // 
            // buttonConnect
            // 
            this.buttonConnect.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.buttonConnect.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.buttonConnect.BorderRadius = 6;
            this.buttonConnect.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonConnect.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonConnect.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.buttonConnect.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.buttonConnect.Location = new System.Drawing.Point(14, 432);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(250, 34);
            this.buttonConnect.TabIndex = 10;
            this.buttonConnect.Text = "连接设备";
            this.buttonConnect.UseAccent = false;
            this.buttonConnect.UseVisualStyleBackColor = false;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // button_StartCAN
            // 
            this.button_StartCAN.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_StartCAN.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_StartCAN.BorderRadius = 6;
            this.button_StartCAN.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_StartCAN.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_StartCAN.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_StartCAN.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_StartCAN.Location = new System.Drawing.Point(14, 472);
            this.button_StartCAN.Name = "button_StartCAN";
            this.button_StartCAN.Size = new System.Drawing.Size(120, 30);
            this.button_StartCAN.TabIndex = 11;
            this.button_StartCAN.Text = "启动 CAN";
            this.button_StartCAN.UseAccent = false;
            this.button_StartCAN.UseVisualStyleBackColor = false;
            this.button_StartCAN.Click += new System.EventHandler(this.button_StartCAN_Click);
            // 
            // button_StopCAN
            // 
            this.button_StopCAN.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_StopCAN.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_StopCAN.BorderRadius = 6;
            this.button_StopCAN.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_StopCAN.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_StopCAN.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_StopCAN.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_StopCAN.Location = new System.Drawing.Point(144, 472);
            this.button_StopCAN.Name = "button_StopCAN";
            this.button_StopCAN.Size = new System.Drawing.Size(120, 30);
            this.button_StopCAN.TabIndex = 12;
            this.button_StopCAN.Text = "复位 CAN";
            this.button_StopCAN.UseAccent = false;
            this.button_StopCAN.UseVisualStyleBackColor = false;
            this.button_StopCAN.Click += new System.EventHandler(this.button_StopCAN_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox4.Controls.Add(this.button_advance);
            this.groupBox4.Controls.Add(this.button_Retreat);
            this.groupBox4.Controls.Add(this.button_Stop);
            this.groupBox4.Controls.Add(this.button_turn_left);
            this.groupBox4.Controls.Add(this.button_turn_right);
            this.groupBox4.Controls.Add(this.trackBar_direction);
            this.groupBox4.Controls.Add(this.label_power);
            this.groupBox4.Controls.Add(this.label_direction);
            this.groupBox4.Controls.Add(this.label16);
            this.groupBox4.Controls.Add(this.label17);
            this.groupBox4.Controls.Add(this.trackBar_Power);
            this.groupBox4.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox4.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox4.Location = new System.Drawing.Point(326, 56);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(569, 514);
            this.groupBox4.TabIndex = 1;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "运动控制";
            // 
            // button_advance
            // 
            this.button_advance.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_advance.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_advance.BorderRadius = 6;
            this.button_advance.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_advance.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_advance.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_advance.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_advance.Location = new System.Drawing.Point(203, 36);
            this.button_advance.Name = "button_advance";
            this.button_advance.Size = new System.Drawing.Size(120, 80);
            this.button_advance.TabIndex = 0;
            this.button_advance.Text = "▲  前进";
            this.button_advance.UseAccent = false;
            this.button_advance.UseVisualStyleBackColor = false;
            this.button_advance.Click += new System.EventHandler(this.button_advance_Click);
            // 
            // button_Retreat
            // 
            this.button_Retreat.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_Retreat.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_Retreat.BorderRadius = 6;
            this.button_Retreat.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_Retreat.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_Retreat.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_Retreat.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_Retreat.Location = new System.Drawing.Point(203, 260);
            this.button_Retreat.Name = "button_Retreat";
            this.button_Retreat.Size = new System.Drawing.Size(120, 80);
            this.button_Retreat.TabIndex = 4;
            this.button_Retreat.Text = "▼  后退";
            this.button_Retreat.UseAccent = false;
            this.button_Retreat.UseVisualStyleBackColor = false;
            this.button_Retreat.Click += new System.EventHandler(this.button_Retreat_Click);
            // 
            // button_Stop
            // 
            this.button_Stop.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_Stop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_Stop.BorderRadius = 6;
            this.button_Stop.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_Stop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_Stop.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_Stop.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_Stop.Location = new System.Drawing.Point(203, 148);
            this.button_Stop.Name = "button_Stop";
            this.button_Stop.Size = new System.Drawing.Size(120, 80);
            this.button_Stop.TabIndex = 1;
            this.button_Stop.Text = "■  停止";
            this.button_Stop.UseAccent = false;
            this.button_Stop.UseVisualStyleBackColor = false;
            this.button_Stop.Click += new System.EventHandler(this.button_Stop_Click);
            // 
            // button_turn_left
            // 
            this.button_turn_left.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_turn_left.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_turn_left.BorderRadius = 6;
            this.button_turn_left.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_turn_left.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_turn_left.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_turn_left.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_turn_left.Location = new System.Drawing.Point(33, 148);
            this.button_turn_left.Name = "button_turn_left";
            this.button_turn_left.Size = new System.Drawing.Size(120, 80);
            this.button_turn_left.TabIndex = 2;
            this.button_turn_left.Text = "◀  左转";
            this.button_turn_left.UseAccent = false;
            this.button_turn_left.UseVisualStyleBackColor = false;
            this.button_turn_left.Click += new System.EventHandler(this.button_turn_left_Click);
            // 
            // button_turn_right
            // 
            this.button_turn_right.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_turn_right.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_turn_right.BorderRadius = 6;
            this.button_turn_right.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_turn_right.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_turn_right.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_turn_right.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_turn_right.Location = new System.Drawing.Point(373, 148);
            this.button_turn_right.Name = "button_turn_right";
            this.button_turn_right.Size = new System.Drawing.Size(120, 80);
            this.button_turn_right.TabIndex = 3;
            this.button_turn_right.Text = "  右转  ▶";
            this.button_turn_right.UseAccent = false;
            this.button_turn_right.UseVisualStyleBackColor = false;
            this.button_turn_right.Click += new System.EventHandler(this.button_turn_right_Click);
            // 
            // trackBar_Power
            // 
            this.trackBar_Power.Location = new System.Drawing.Point(506, 60);
            this.trackBar_Power.Maximum = 100;
            this.trackBar_Power.Name = "trackBar_Power";
            this.trackBar_Power.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar_Power.Size = new System.Drawing.Size(69, 280);
            this.trackBar_Power.TabIndex = 5;
            this.trackBar_Power.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar_Power.Scroll += new System.EventHandler(this.trackBar_Power_Scroll);
            // 
            // trackBar_direction
            // 
            this.trackBar_direction.Location = new System.Drawing.Point(50, 394);
            this.trackBar_direction.Maximum = 100;
            this.trackBar_direction.Name = "trackBar_direction";
            this.trackBar_direction.Size = new System.Drawing.Size(340, 69);
            this.trackBar_direction.TabIndex = 6;
            this.trackBar_direction.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar_direction.Scroll += new System.EventHandler(this.trackBar_direction_Scroll);
            // 
            // label_power
            // 
            this.label_power.AutoSize = true;
            this.label_power.Font = new System.Drawing.Font("Segoe UI", 11F, System.Drawing.FontStyle.Bold);
            this.label_power.Location = new System.Drawing.Point(501, 343);
            this.label_power.Name = "label_power";
            this.label_power.Size = new System.Drawing.Size(45, 30);
            this.label_power.TabIndex = 7;
            this.label_power.Text = "0%";
            // 
            // label_direction
            // 
            this.label_direction.AutoSize = true;
            this.label_direction.Font = new System.Drawing.Font("Segoe UI", 11F, System.Drawing.FontStyle.Bold);
            this.label_direction.Location = new System.Drawing.Point(138, 368);
            this.label_direction.Name = "label_direction";
            this.label_direction.Size = new System.Drawing.Size(45, 30);
            this.label_direction.TabIndex = 8;
            this.label_direction.Text = "0%";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(501, 36);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(52, 28);
            this.label16.TabIndex = 9;
            this.label16.Text = "油门";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(50, 370);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(92, 28);
            this.label17.TabIndex = 10;
            this.label17.Text = "转向力度";
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox3.Controls.Add(this.textBox_motor1);
            this.groupBox3.Controls.Add(this.textBox_motor2);
            this.groupBox3.Controls.Add(this.button_brake_state1);
            this.groupBox3.Controls.Add(this.button_brake_state2);
            this.groupBox3.Controls.Add(this.textBox_Remote2);
            this.groupBox3.Controls.Add(this.textBox_Remote4);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox3.Location = new System.Drawing.Point(917, 56);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(431, 514);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "底盘遥测";
            // 
            // textBox_motor1
            // 
            this.textBox_motor1.Font = new System.Drawing.Font("Segoe UI", 26F, System.Drawing.FontStyle.Bold);
            this.textBox_motor1.Location = new System.Drawing.Point(19, 76);
            this.textBox_motor1.Name = "textBox_motor1";
            this.textBox_motor1.ReadOnly = true;
            this.textBox_motor1.Size = new System.Drawing.Size(185, 77);
            this.textBox_motor1.TabIndex = 1;
            this.textBox_motor1.Text = "0";
            this.textBox_motor1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // textBox_motor2
            // 
            this.textBox_motor2.Font = new System.Drawing.Font("Segoe UI", 26F, System.Drawing.FontStyle.Bold);
            this.textBox_motor2.Location = new System.Drawing.Point(19, 203);
            this.textBox_motor2.Name = "textBox_motor2";
            this.textBox_motor2.ReadOnly = true;
            this.textBox_motor2.Size = new System.Drawing.Size(185, 77);
            this.textBox_motor2.TabIndex = 2;
            this.textBox_motor2.Text = "0";
            this.textBox_motor2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // button_brake_state1
            // 
            this.button_brake_state1.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_brake_state1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_brake_state1.BorderRadius = 6;
            this.button_brake_state1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_brake_state1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_brake_state1.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold);
            this.button_brake_state1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_brake_state1.Location = new System.Drawing.Point(239, 85);
            this.button_brake_state1.Name = "button_brake_state1";
            this.button_brake_state1.Size = new System.Drawing.Size(110, 53);
            this.button_brake_state1.TabIndex = 3;
            this.button_brake_state1.Text = "解除";
            this.button_brake_state1.UseAccent = false;
            this.button_brake_state1.UseVisualStyleBackColor = false;
            // 
            // button_brake_state2
            // 
            this.button_brake_state2.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_brake_state2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_brake_state2.BorderRadius = 6;
            this.button_brake_state2.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_brake_state2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_brake_state2.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold);
            this.button_brake_state2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_brake_state2.Location = new System.Drawing.Point(239, 206);
            this.button_brake_state2.Name = "button_brake_state2";
            this.button_brake_state2.Size = new System.Drawing.Size(110, 53);
            this.button_brake_state2.TabIndex = 4;
            this.button_brake_state2.Text = "解除";
            this.button_brake_state2.UseAccent = false;
            this.button_brake_state2.UseVisualStyleBackColor = false;
            // 
            // textBox_Remote2
            // 
            this.textBox_Remote2.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Bold);
            this.textBox_Remote2.Location = new System.Drawing.Point(19, 330);
            this.textBox_Remote2.Name = "textBox_Remote2";
            this.textBox_Remote2.ReadOnly = true;
            this.textBox_Remote2.Size = new System.Drawing.Size(185, 55);
            this.textBox_Remote2.TabIndex = 5;
            this.textBox_Remote2.Text = "0";
            this.textBox_Remote2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // textBox_Remote4
            // 
            this.textBox_Remote4.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Bold);
            this.textBox_Remote4.Location = new System.Drawing.Point(244, 327);
            this.textBox_Remote4.Name = "textBox_Remote4";
            this.textBox_Remote4.ReadOnly = true;
            this.textBox_Remote4.Size = new System.Drawing.Size(180, 55);
            this.textBox_Remote4.TabIndex = 6;
            this.textBox_Remote4.Text = "0";
            this.textBox_Remote4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label10.Location = new System.Drawing.Point(19, 36);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(129, 30);
            this.label10.TabIndex = 7;
            this.label10.Text = "电机 1 转速";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label11.Location = new System.Drawing.Point(19, 163);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(129, 30);
            this.label11.TabIndex = 8;
            this.label11.Text = "电机 2 转速";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label12.Location = new System.Drawing.Point(19, 290);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(189, 30);
            this.label12.TabIndex = 9;
            this.label12.Text = "遥控通道 2 (前后)";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label13.Location = new System.Drawing.Point(239, 290);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(189, 30);
            this.label13.TabIndex = 10;
            this.label13.Text = "遥控通道 4 (转向)";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label14.Location = new System.Drawing.Point(239, 36);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(100, 30);
            this.label14.TabIndex = 11;
            this.label14.Text = "制动器 1";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Segoe UI", 11F);
            this.label15.Location = new System.Drawing.Point(239, 157);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(100, 30);
            this.label15.TabIndex = 12;
            this.label15.Text = "制动器 2";
            // 
            // groupBox5
            // 
            this.groupBox5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox5.Controls.Add(this.comboBox_FrameFormat);
            this.groupBox5.Controls.Add(this.comboBox_FrameType);
            this.groupBox5.Controls.Add(this.textBox_ID);
            this.groupBox5.Controls.Add(this.textBox_Data);
            this.groupBox5.Controls.Add(this.button_Send);
            this.groupBox5.Controls.Add(this.button_Clear);
            this.groupBox5.Controls.Add(this.label18);
            this.groupBox5.Controls.Add(this.label19);
            this.groupBox5.Controls.Add(this.label20);
            this.groupBox5.Controls.Add(this.label21);
            this.groupBox5.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox5.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox5.Location = new System.Drawing.Point(12, 586);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(1336, 85);
            this.groupBox5.TabIndex = 3;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "发送数据帧";
            // 
            // comboBox_FrameFormat
            // 
            this.comboBox_FrameFormat.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_FrameFormat.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_FrameFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_FrameFormat.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_FrameFormat.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_FrameFormat.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_FrameFormat.ItemHeight = 26;
            this.comboBox_FrameFormat.Location = new System.Drawing.Point(66, 26);
            this.comboBox_FrameFormat.Name = "comboBox_FrameFormat";
            this.comboBox_FrameFormat.Size = new System.Drawing.Size(90, 32);
            this.comboBox_FrameFormat.TabIndex = 0;
            // 
            // comboBox_FrameType
            // 
            this.comboBox_FrameType.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(40)))), ((int)(((byte)(58)))));
            this.comboBox_FrameType.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBox_FrameType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_FrameType.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.comboBox_FrameType.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.comboBox_FrameType.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.comboBox_FrameType.ItemHeight = 26;
            this.comboBox_FrameType.Location = new System.Drawing.Point(244, 26);
            this.comboBox_FrameType.Name = "comboBox_FrameType";
            this.comboBox_FrameType.Size = new System.Drawing.Size(90, 32);
            this.comboBox_FrameType.TabIndex = 1;
            // 
            // textBox_ID
            // 
            this.textBox_ID.Location = new System.Drawing.Point(455, 32);
            this.textBox_ID.Name = "textBox_ID";
            this.textBox_ID.Size = new System.Drawing.Size(70, 34);
            this.textBox_ID.TabIndex = 2;
            // 
            // textBox_Data
            // 
            this.textBox_Data.Location = new System.Drawing.Point(592, 32);
            this.textBox_Data.Name = "textBox_Data";
            this.textBox_Data.Size = new System.Drawing.Size(487, 34);
            this.textBox_Data.TabIndex = 3;
            // 
            // button_Send
            // 
            this.button_Send.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_Send.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_Send.BorderRadius = 6;
            this.button_Send.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_Send.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_Send.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_Send.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_Send.Location = new System.Drawing.Point(1098, 36);
            this.button_Send.Name = "button_Send";
            this.button_Send.Size = new System.Drawing.Size(100, 30);
            this.button_Send.TabIndex = 4;
            this.button_Send.Text = "发送";
            this.button_Send.UseAccent = false;
            this.button_Send.UseVisualStyleBackColor = false;
            this.button_Send.Click += new System.EventHandler(this.button_Send_Click);
            // 
            // button_Clear
            // 
            this.button_Clear.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.button_Clear.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(38)))), ((int)(((byte)(56)))));
            this.button_Clear.BorderRadius = 6;
            this.button_Clear.Cursor = System.Windows.Forms.Cursors.Hand;
            this.button_Clear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button_Clear.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.button_Clear.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(237)))), ((int)(((byte)(243)))));
            this.button_Clear.Location = new System.Drawing.Point(1215, 36);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(100, 30);
            this.button_Clear.TabIndex = 5;
            this.button_Clear.Text = "清空日志";
            this.button_Clear.UseAccent = false;
            this.button_Clear.UseVisualStyleBackColor = false;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(372, 37);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(79, 28);
            this.label18.TabIndex = 6;
            this.label18.Text = "帧ID 0X";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(181, 37);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(52, 28);
            this.label19.TabIndex = 7;
            this.label19.Text = "类型";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(532, 37);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(52, 28);
            this.label20.TabIndex = 8;
            this.label20.Text = "数据";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(9, 37);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(52, 28);
            this.label21.TabIndex = 9;
            this.label21.Text = "格式";
            // 
            // groupBox6
            // 
            this.groupBox6.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(26)))), ((int)(((byte)(32)))), ((int)(((byte)(48)))));
            this.groupBox6.Controls.Add(this.listView_Info);
            this.groupBox6.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.groupBox6.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(75)))), ((int)(((byte)(159)))), ((int)(((byte)(255)))));
            this.groupBox6.Location = new System.Drawing.Point(12, 681);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(1336, 315);
            this.groupBox6.TabIndex = 4;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "CAN 消息日志";
            // 
            // listView_Info
            // 
            this.listView_Info.HideSelection = false;
            this.listView_Info.Location = new System.Drawing.Point(14, 43);
            this.listView_Info.Name = "listView_Info";
            this.listView_Info.Size = new System.Drawing.Size(1306, 255);
            this.listView_Info.TabIndex = 0;
            this.listView_Info.UseCompatibleStateImageBehavior = false;
            this.listView_Info.View = System.Windows.Forms.View.Details;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1360, 1008);
            this.Controls.Add(this.panelStatusBar);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox6);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(1200, 800);
            this.Name = "Form1";
            this.Text = "线控底盘监测控制系统";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panelStatusBar.ResumeLayout(false);
            this.panelStatusBar.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Power)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_direction)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        // ═══ 控件声明 ═══
        private System.Windows.Forms.Timer timer_rec;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Panel panelStatusBar;
        private System.Windows.Forms.Label labelStatusConn;
        private System.Windows.Forms.Label labelStatusCAN;
        private System.Windows.Forms.Label labelStatusDev;
        private System.Windows.Forms.Label labelStatusCh;

        private Controls.ModernButton buttonConnect;
        private Controls.ModernButton button_StartCAN;
        private Controls.ModernButton button_StopCAN;
        private Controls.ModernButton button_Clear;
        private Controls.ModernComboBox comboBox_devtype;
        private Controls.ModernComboBox comboBox_DevIndex;
        private Controls.ModernComboBox comboBox_CANIndex;
        private Controls.ModernComboBox comboBox_Filter;
        private Controls.ModernComboBox comboBox_Mode;
        private Controls.ModernComboBox comboBox_FrameType;
        private Controls.ModernComboBox comboBox_FrameFormat;
        private System.Windows.Forms.TextBox textBox_Data;
        private System.Windows.Forms.TextBox textBox_ID;
        private System.Windows.Forms.TextBox textBox_Time0;
        private System.Windows.Forms.TextBox textBox_Time1;
        private System.Windows.Forms.TextBox textBox_AccCode;
        private System.Windows.Forms.TextBox textBox_AccMask;

        private Controls.ModernButton button_Send;
        private Controls.ModernButton button_advance;
        private Controls.ModernButton button_Retreat;
        private Controls.ModernButton button_Stop;
        private System.Windows.Forms.TrackBar trackBar_Power;
        private Controls.ModernButton button_turn_left;
        private Controls.ModernButton button_turn_right;
        private System.Windows.Forms.TrackBar trackBar_direction;

        private System.Windows.Forms.TextBox textBox_motor1;
        private System.Windows.Forms.TextBox textBox_motor2;
        private Controls.ModernButton button_brake_state1;
        private Controls.ModernButton button_brake_state2;
        private System.Windows.Forms.TextBox textBox_Remote2;
        private System.Windows.Forms.TextBox textBox_Remote4;

        private System.Windows.Forms.ListView listView_Info;

        private Controls.ModernGroupBox groupBox1;
        private Controls.ModernGroupBox groupBox2;
        private Controls.ModernGroupBox groupBox3;
        private Controls.ModernGroupBox groupBox4;
        private Controls.ModernGroupBox groupBox5;
        private Controls.ModernGroupBox groupBox6;

        private System.Windows.Forms.Label label1, label2, label3, label4, label5;
        private System.Windows.Forms.Label label6, label8, label9, label7;
        private System.Windows.Forms.Label label10, label11, label12, label13, label14, label15;
        private System.Windows.Forms.Label label16, label17;
        private System.Windows.Forms.Label label18, label19, label20, label21;
        private System.Windows.Forms.Label label_power, label_direction;
    }
}
