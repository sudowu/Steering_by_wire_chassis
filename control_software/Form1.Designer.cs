namespace control_software
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
            this.buttonConnect = new System.Windows.Forms.Button();
            this.button_StartCAN = new System.Windows.Forms.Button();
            this.button_StopCAN = new System.Windows.Forms.Button();
            this.button_Clear = new System.Windows.Forms.Button();
            this.comboBox_devtype = new System.Windows.Forms.ComboBox();
            this.comboBox_DevIndex = new System.Windows.Forms.ComboBox();
            this.comboBox_CANIndex = new System.Windows.Forms.ComboBox();
            this.comboBox_Filter = new System.Windows.Forms.ComboBox();
            this.comboBox_Mode = new System.Windows.Forms.ComboBox();
            this.comboBox_FrameType = new System.Windows.Forms.ComboBox();
            this.comboBox_FrameFormat = new System.Windows.Forms.ComboBox();
            this.textBox_Data = new System.Windows.Forms.TextBox();
            this.button_Send = new System.Windows.Forms.Button();
            this.timer_rec = new System.Windows.Forms.Timer(this.components);
            this.textBox_ID = new System.Windows.Forms.TextBox();
            this.textBox_Time0 = new System.Windows.Forms.TextBox();
            this.textBox_Time1 = new System.Windows.Forms.TextBox();
            this.textBox_AccCode = new System.Windows.Forms.TextBox();
            this.textBox_AccMask = new System.Windows.Forms.TextBox();
            this.listView_Info = new System.Windows.Forms.ListView();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.button_advance = new System.Windows.Forms.Button();
            this.button_Retreat = new System.Windows.Forms.Button();
            this.button_Stop = new System.Windows.Forms.Button();
            this.trackBar_Power = new System.Windows.Forms.TrackBar();
            this.button_turn_left = new System.Windows.Forms.Button();
            this.button_turn_right = new System.Windows.Forms.Button();
            this.trackBar_direction = new System.Windows.Forms.TrackBar();
            this.textBox_motor1 = new System.Windows.Forms.TextBox();
            this.textBox_motor2 = new System.Windows.Forms.TextBox();
            this.button_brake_state1 = new System.Windows.Forms.Button();
            this.button_brake_state2 = new System.Windows.Forms.Button();
            this.textBox_Remote2 = new System.Windows.Forms.TextBox();
            this.textBox_Remote4 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label_direction = new System.Windows.Forms.Label();
            this.label_power = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Power)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_direction)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(23, 12);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(120, 46);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "连接";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // button_StartCAN
            // 
            this.button_StartCAN.Location = new System.Drawing.Point(193, 12);
            this.button_StartCAN.Name = "button_StartCAN";
            this.button_StartCAN.Size = new System.Drawing.Size(120, 46);
            this.button_StartCAN.TabIndex = 0;
            this.button_StartCAN.Text = "启动CAN";
            this.button_StartCAN.UseVisualStyleBackColor = true;
            this.button_StartCAN.Click += new System.EventHandler(this.button_StartCAN_Click);
            // 
            // button_StopCAN
            // 
            this.button_StopCAN.Location = new System.Drawing.Point(351, 12);
            this.button_StopCAN.Name = "button_StopCAN";
            this.button_StopCAN.Size = new System.Drawing.Size(120, 46);
            this.button_StopCAN.TabIndex = 0;
            this.button_StopCAN.Text = "复位CAN";
            this.button_StopCAN.UseVisualStyleBackColor = true;
            this.button_StopCAN.Click += new System.EventHandler(this.button_StopCAN_Click);
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(501, 12);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(120, 46);
            this.button_Clear.TabIndex = 0;
            this.button_Clear.Text = "清空列表";
            this.button_Clear.UseVisualStyleBackColor = true;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
            // 
            // comboBox_devtype
            // 
            this.comboBox_devtype.FormattingEnabled = true;
            this.comboBox_devtype.Items.AddRange(new object[] {
            "3",
            "4"});
            this.comboBox_devtype.Location = new System.Drawing.Point(62, 50);
            this.comboBox_devtype.Name = "comboBox_devtype";
            this.comboBox_devtype.Size = new System.Drawing.Size(129, 26);
            this.comboBox_devtype.TabIndex = 2;
            this.comboBox_devtype.Text = "类型";
            // 
            // comboBox_DevIndex
            // 
            this.comboBox_DevIndex.FormattingEnabled = true;
            this.comboBox_DevIndex.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3"});
            this.comboBox_DevIndex.Location = new System.Drawing.Point(279, 53);
            this.comboBox_DevIndex.Name = "comboBox_DevIndex";
            this.comboBox_DevIndex.Size = new System.Drawing.Size(100, 26);
            this.comboBox_DevIndex.TabIndex = 3;
            this.comboBox_DevIndex.Text = "索引号";
            // 
            // comboBox_CANIndex
            // 
            this.comboBox_CANIndex.FormattingEnabled = true;
            this.comboBox_CANIndex.Items.AddRange(new object[] {
            "0",
            "1"});
            this.comboBox_CANIndex.Location = new System.Drawing.Point(478, 53);
            this.comboBox_CANIndex.Name = "comboBox_CANIndex";
            this.comboBox_CANIndex.Size = new System.Drawing.Size(120, 26);
            this.comboBox_CANIndex.TabIndex = 4;
            this.comboBox_CANIndex.Text = "通道";
            this.comboBox_CANIndex.SelectedIndexChanged += new System.EventHandler(this.comboBox_CANIndex_SelectedIndexChanged);
            // 
            // comboBox_Filter
            // 
            this.comboBox_Filter.FormattingEnabled = true;
            this.comboBox_Filter.Items.AddRange(new object[] {
            "接收全部类型",
            "只接收标准帧",
            "只接收扩展帧"});
            this.comboBox_Filter.Location = new System.Drawing.Point(469, 93);
            this.comboBox_Filter.Name = "comboBox_Filter";
            this.comboBox_Filter.Size = new System.Drawing.Size(120, 26);
            this.comboBox_Filter.TabIndex = 4;
            this.comboBox_Filter.Text = "滤波方式";
            // 
            // comboBox_Mode
            // 
            this.comboBox_Mode.FormattingEnabled = true;
            this.comboBox_Mode.Items.AddRange(new object[] {
            "正常",
            "只听",
            "自测"});
            this.comboBox_Mode.Location = new System.Drawing.Point(469, 41);
            this.comboBox_Mode.Name = "comboBox_Mode";
            this.comboBox_Mode.Size = new System.Drawing.Size(120, 26);
            this.comboBox_Mode.TabIndex = 4;
            this.comboBox_Mode.Text = "模式";
            // 
            // comboBox_FrameType
            // 
            this.comboBox_FrameType.FormattingEnabled = true;
            this.comboBox_FrameType.Items.AddRange(new object[] {
            "标准帧",
            "扩展帧"});
            this.comboBox_FrameType.Location = new System.Drawing.Point(254, 27);
            this.comboBox_FrameType.Name = "comboBox_FrameType";
            this.comboBox_FrameType.Size = new System.Drawing.Size(90, 26);
            this.comboBox_FrameType.TabIndex = 4;
            this.comboBox_FrameType.Text = "帧类型";
            // 
            // comboBox_FrameFormat
            // 
            this.comboBox_FrameFormat.FormattingEnabled = true;
            this.comboBox_FrameFormat.Items.AddRange(new object[] {
            "数据帧",
            "远程帧"});
            this.comboBox_FrameFormat.Location = new System.Drawing.Point(94, 27);
            this.comboBox_FrameFormat.Name = "comboBox_FrameFormat";
            this.comboBox_FrameFormat.Size = new System.Drawing.Size(90, 26);
            this.comboBox_FrameFormat.TabIndex = 4;
            this.comboBox_FrameFormat.Text = "帧格式";
            // 
            // textBox_Data
            // 
            this.textBox_Data.Location = new System.Drawing.Point(94, 74);
            this.textBox_Data.Name = "textBox_Data";
            this.textBox_Data.Size = new System.Drawing.Size(384, 28);
            this.textBox_Data.TabIndex = 5;
            // 
            // button_Send
            // 
            this.button_Send.Location = new System.Drawing.Point(495, 27);
            this.button_Send.Name = "button_Send";
            this.button_Send.Size = new System.Drawing.Size(101, 79);
            this.button_Send.TabIndex = 7;
            this.button_Send.Text = "发送";
            this.button_Send.UseVisualStyleBackColor = true;
            this.button_Send.Click += new System.EventHandler(this.button_Send_Click);
            // 
            // timer_rec
            // 
            this.timer_rec.Tick += new System.EventHandler(this.timer_rec_Tick);
            // 
            // textBox_ID
            // 
            this.textBox_ID.Location = new System.Drawing.Point(399, 27);
            this.textBox_ID.Name = "textBox_ID";
            this.textBox_ID.Size = new System.Drawing.Size(79, 28);
            this.textBox_ID.TabIndex = 8;
            this.textBox_ID.Text = "帧ID";
            // 
            // textBox_Time0
            // 
            this.textBox_Time0.Location = new System.Drawing.Point(270, 39);
            this.textBox_Time0.Name = "textBox_Time0";
            this.textBox_Time0.Size = new System.Drawing.Size(100, 28);
            this.textBox_Time0.TabIndex = 8;
            this.textBox_Time0.Text = "定时器0";
            // 
            // textBox_Time1
            // 
            this.textBox_Time1.Location = new System.Drawing.Point(270, 91);
            this.textBox_Time1.Name = "textBox_Time1";
            this.textBox_Time1.Size = new System.Drawing.Size(100, 28);
            this.textBox_Time1.TabIndex = 9;
            this.textBox_Time1.Text = "定时器1";
            // 
            // textBox_AccCode
            // 
            this.textBox_AccCode.Location = new System.Drawing.Point(83, 38);
            this.textBox_AccCode.Name = "textBox_AccCode";
            this.textBox_AccCode.Size = new System.Drawing.Size(100, 28);
            this.textBox_AccCode.TabIndex = 8;
            this.textBox_AccCode.Text = "验收码";
            // 
            // textBox_AccMask
            // 
            this.textBox_AccMask.Location = new System.Drawing.Point(83, 95);
            this.textBox_AccMask.Name = "textBox_AccMask";
            this.textBox_AccMask.Size = new System.Drawing.Size(100, 28);
            this.textBox_AccMask.TabIndex = 9;
            this.textBox_AccMask.Text = "屏蔽码";
            // 
            // listView_Info
            // 
            this.listView_Info.HideSelection = false;
            this.listView_Info.Location = new System.Drawing.Point(18, 31);
            this.listView_Info.Name = "listView_Info";
            this.listView_Info.Size = new System.Drawing.Size(592, 395);
            this.listView_Info.TabIndex = 10;
            this.listView_Info.UseCompatibleStateImageBehavior = false;
            this.listView_Info.View = System.Windows.Forms.View.Details;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "red_light.png");
            this.imageList1.Images.SetKeyName(1, "green_light.png");
            // 
            // button_advance
            // 
            this.button_advance.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.button_advance.Location = new System.Drawing.Point(218, 109);
            this.button_advance.Name = "button_advance";
            this.button_advance.Size = new System.Drawing.Size(96, 60);
            this.button_advance.TabIndex = 12;
            this.button_advance.Text = "前进";
            this.button_advance.UseVisualStyleBackColor = false;
            this.button_advance.Click += new System.EventHandler(this.button_advance_Click);
            // 
            // button_Retreat
            // 
            this.button_Retreat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.button_Retreat.Location = new System.Drawing.Point(218, 298);
            this.button_Retreat.Name = "button_Retreat";
            this.button_Retreat.Size = new System.Drawing.Size(96, 60);
            this.button_Retreat.TabIndex = 12;
            this.button_Retreat.Text = "后退";
            this.button_Retreat.UseVisualStyleBackColor = false;
            this.button_Retreat.Click += new System.EventHandler(this.button_Retreat_Click);
            // 
            // button_Stop
            // 
            this.button_Stop.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.button_Stop.Location = new System.Drawing.Point(218, 194);
            this.button_Stop.Name = "button_Stop";
            this.button_Stop.Size = new System.Drawing.Size(96, 60);
            this.button_Stop.TabIndex = 12;
            this.button_Stop.Text = " 停止";
            this.button_Stop.UseVisualStyleBackColor = false;
            this.button_Stop.Click += new System.EventHandler(this.button_Stop_Click);
            // 
            // trackBar_Power
            // 
            this.trackBar_Power.Location = new System.Drawing.Point(518, 109);
            this.trackBar_Power.Maximum = 100;
            this.trackBar_Power.Name = "trackBar_Power";
            this.trackBar_Power.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar_Power.Size = new System.Drawing.Size(69, 303);
            this.trackBar_Power.TabIndex = 13;
            this.trackBar_Power.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar_Power.Scroll += new System.EventHandler(this.trackBar_Power_Scroll);
            // 
            // button_turn_left
            // 
            this.button_turn_left.Location = new System.Drawing.Point(83, 194);
            this.button_turn_left.Name = "button_turn_left";
            this.button_turn_left.Size = new System.Drawing.Size(96, 60);
            this.button_turn_left.TabIndex = 14;
            this.button_turn_left.Text = "左转";
            this.button_turn_left.UseVisualStyleBackColor = true;
            this.button_turn_left.Click += new System.EventHandler(this.button_turn_left_Click);
            // 
            // button_turn_right
            // 
            this.button_turn_right.Location = new System.Drawing.Point(349, 194);
            this.button_turn_right.Name = "button_turn_right";
            this.button_turn_right.Size = new System.Drawing.Size(96, 60);
            this.button_turn_right.TabIndex = 14;
            this.button_turn_right.Text = "右转";
            this.button_turn_right.UseVisualStyleBackColor = true;
            this.button_turn_right.Click += new System.EventHandler(this.button_turn_right_Click);
            // 
            // trackBar_direction
            // 
            this.trackBar_direction.Location = new System.Drawing.Point(62, 46);
            this.trackBar_direction.Maximum = 100;
            this.trackBar_direction.Name = "trackBar_direction";
            this.trackBar_direction.Size = new System.Drawing.Size(307, 69);
            this.trackBar_direction.TabIndex = 15;
            this.trackBar_direction.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar_direction.Scroll += new System.EventHandler(this.trackBar_direction_Scroll);
            // 
            // textBox_motor1
            // 
            this.textBox_motor1.Location = new System.Drawing.Point(24, 73);
            this.textBox_motor1.Name = "textBox_motor1";
            this.textBox_motor1.Size = new System.Drawing.Size(107, 28);
            this.textBox_motor1.TabIndex = 16;
            this.textBox_motor1.Text = "电机1";
            // 
            // textBox_motor2
            // 
            this.textBox_motor2.Location = new System.Drawing.Point(24, 163);
            this.textBox_motor2.Name = "textBox_motor2";
            this.textBox_motor2.Size = new System.Drawing.Size(104, 28);
            this.textBox_motor2.TabIndex = 16;
            this.textBox_motor2.Text = "电机2";
            // 
            // button_brake_state1
            // 
            this.button_brake_state1.Location = new System.Drawing.Point(331, 70);
            this.button_brake_state1.Name = "button_brake_state1";
            this.button_brake_state1.Size = new System.Drawing.Size(116, 121);
            this.button_brake_state1.TabIndex = 17;
            this.button_brake_state1.Text = "制动状态";
            this.button_brake_state1.UseVisualStyleBackColor = true;
            // 
            // button_brake_state2
            // 
            this.button_brake_state2.Location = new System.Drawing.Point(474, 72);
            this.button_brake_state2.Name = "button_brake_state2";
            this.button_brake_state2.Size = new System.Drawing.Size(116, 119);
            this.button_brake_state2.TabIndex = 17;
            this.button_brake_state2.Text = "制动状态";
            this.button_brake_state2.UseVisualStyleBackColor = true;
            // 
            // textBox_Remote2
            // 
            this.textBox_Remote2.Location = new System.Drawing.Point(169, 72);
            this.textBox_Remote2.Name = "textBox_Remote2";
            this.textBox_Remote2.Size = new System.Drawing.Size(116, 28);
            this.textBox_Remote2.TabIndex = 16;
            this.textBox_Remote2.Text = "前后遥控信号";
            // 
            // textBox_Remote4
            // 
            this.textBox_Remote4.Location = new System.Drawing.Point(166, 163);
            this.textBox_Remote4.Name = "textBox_Remote4";
            this.textBox_Remote4.Size = new System.Drawing.Size(116, 28);
            this.textBox_Remote4.TabIndex = 16;
            this.textBox_Remote4.Text = "方向遥控信号";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.comboBox_devtype);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.comboBox_CANIndex);
            this.groupBox1.Controls.Add(this.comboBox_DevIndex);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(23, 82);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(624, 270);
            this.groupBox1.TabIndex = 18;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "设备参数";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.comboBox_Filter);
            this.groupBox2.Controls.Add(this.textBox_AccMask);
            this.groupBox2.Controls.Add(this.comboBox_Mode);
            this.groupBox2.Controls.Add(this.textBox_Time1);
            this.groupBox2.Controls.Add(this.textBox_Time0);
            this.groupBox2.Controls.Add(this.textBox_AccCode);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Location = new System.Drawing.Point(9, 93);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(609, 157);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "初始化CAN参数";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 98);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(80, 18);
            this.label5.TabIndex = 11;
            this.label5.Text = "屏蔽码：";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(195, 96);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 18);
            this.label7.TabIndex = 11;
            this.label7.Text = "定时器1：";
            this.label7.Click += new System.EventHandler(this.label6_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(376, 98);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(98, 18);
            this.label9.TabIndex = 11;
            this.label9.Text = "滤波方式：";
            this.label9.Click += new System.EventHandler(this.label6_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(412, 44);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(62, 18);
            this.label8.TabIndex = 11;
            this.label8.Text = "模式：";
            this.label8.Click += new System.EventHandler(this.label6_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(195, 44);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(89, 18);
            this.label6.TabIndex = 11;
            this.label6.Text = "定时器0：";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 41);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 18);
            this.label4.TabIndex = 11;
            this.label4.Text = "验收码：";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(421, 56);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(62, 18);
            this.label3.TabIndex = 11;
            this.label3.Text = "通道：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(204, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 18);
            this.label2.TabIndex = 11;
            this.label2.Text = "索引号：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 18);
            this.label1.TabIndex = 11;
            this.label1.Text = "类型：";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.button_brake_state2);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.textBox_motor1);
            this.groupBox3.Controls.Add(this.textBox_Remote2);
            this.groupBox3.Controls.Add(this.textBox_Remote4);
            this.groupBox3.Controls.Add(this.textBox_motor2);
            this.groupBox3.Controls.Add(this.button_brake_state1);
            this.groupBox3.Location = new System.Drawing.Point(665, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(616, 212);
            this.groupBox3.TabIndex = 19;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "底盘状态";
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(166, 133);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(116, 18);
            this.label13.TabIndex = 19;
            this.label13.Text = "方向遥控通道";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(471, 41);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(125, 18);
            this.label15.TabIndex = 19;
            this.label15.Text = "电机2制动状态";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(328, 41);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(125, 18);
            this.label14.TabIndex = 19;
            this.label14.Text = "电机1制动状态";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(166, 41);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(116, 18);
            this.label12.TabIndex = 19;
            this.label12.Text = "前后遥控通道";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(21, 133);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(89, 18);
            this.label11.TabIndex = 18;
            this.label11.Text = "电机2数值";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(21, 41);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(89, 18);
            this.label10.TabIndex = 18;
            this.label10.Text = "电机1数值";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label17);
            this.groupBox4.Controls.Add(this.label_direction);
            this.groupBox4.Controls.Add(this.label_power);
            this.groupBox4.Controls.Add(this.label16);
            this.groupBox4.Controls.Add(this.trackBar_direction);
            this.groupBox4.Controls.Add(this.trackBar_Power);
            this.groupBox4.Controls.Add(this.button_advance);
            this.groupBox4.Controls.Add(this.button_Retreat);
            this.groupBox4.Controls.Add(this.button_Stop);
            this.groupBox4.Controls.Add(this.button_turn_left);
            this.groupBox4.Controls.Add(this.button_turn_right);
            this.groupBox4.Location = new System.Drawing.Point(23, 367);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(624, 442);
            this.groupBox4.TabIndex = 20;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "底盘控制";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(385, 33);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(80, 18);
            this.label17.TabIndex = 18;
            this.label17.Text = "转向速度";
            // 
            // label_direction
            // 
            this.label_direction.AutoSize = true;
            this.label_direction.Location = new System.Drawing.Point(414, 55);
            this.label_direction.Name = "label_direction";
            this.label_direction.Size = new System.Drawing.Size(26, 18);
            this.label_direction.TabIndex = 17;
            this.label_direction.Text = "0%";
            this.label_direction.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label_power
            // 
            this.label_power.AutoSize = true;
            this.label_power.Location = new System.Drawing.Point(524, 88);
            this.label_power.Name = "label_power";
            this.label_power.Size = new System.Drawing.Size(26, 18);
            this.label_power.TabIndex = 17;
            this.label_power.Text = "0%";
            this.label_power.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(515, 65);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(44, 18);
            this.label16.TabIndex = 16;
            this.label16.Text = "油门";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.comboBox_FrameFormat);
            this.groupBox5.Controls.Add(this.comboBox_FrameType);
            this.groupBox5.Controls.Add(this.textBox_Data);
            this.groupBox5.Controls.Add(this.button_Send);
            this.groupBox5.Controls.Add(this.textBox_ID);
            this.groupBox5.Controls.Add(this.label21);
            this.groupBox5.Controls.Add(this.label20);
            this.groupBox5.Controls.Add(this.label19);
            this.groupBox5.Controls.Add(this.label18);
            this.groupBox5.Location = new System.Drawing.Point(665, 230);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(616, 125);
            this.groupBox5.TabIndex = 21;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "发送数据帧";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.listView_Info);
            this.groupBox6.Location = new System.Drawing.Point(665, 367);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(616, 442);
            this.groupBox6.TabIndex = 22;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "信息";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(350, 30);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(62, 18);
            this.label18.TabIndex = 11;
            this.label18.Text = "帧ID：";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(185, 30);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(80, 18);
            this.label19.TabIndex = 11;
            this.label19.Text = "帧类型：";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(26, 77);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(62, 18);
            this.label20.TabIndex = 11;
            this.label20.Text = "数据：";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(26, 30);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(80, 18);
            this.label21.TabIndex = 11;
            this.label21.Text = "帧格式：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1302, 821);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.button_StartCAN);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.button_StopCAN);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "线控底盘监测控制系统";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_Power)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar_direction)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button button_StartCAN;
        private System.Windows.Forms.Button button_StopCAN;
        private System.Windows.Forms.Button button_Clear;
        private System.Windows.Forms.ComboBox comboBox_devtype;
        private System.Windows.Forms.ComboBox comboBox_DevIndex;
        private System.Windows.Forms.ComboBox comboBox_CANIndex;
        private System.Windows.Forms.ComboBox comboBox_Filter;
        private System.Windows.Forms.ComboBox comboBox_Mode;
        private System.Windows.Forms.ComboBox comboBox_FrameType;
        private System.Windows.Forms.ComboBox comboBox_FrameFormat;
        private System.Windows.Forms.TextBox textBox_Data;
        private System.Windows.Forms.Button button_Send;
        private System.Windows.Forms.Timer timer_rec;
        private System.Windows.Forms.TextBox textBox_ID;
        private System.Windows.Forms.TextBox textBox_Time0;
        private System.Windows.Forms.TextBox textBox_Time1;
        private System.Windows.Forms.TextBox textBox_AccCode;
        private System.Windows.Forms.TextBox textBox_AccMask;
        private System.Windows.Forms.ListView listView_Info;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Button button_advance;
        private System.Windows.Forms.Button button_Retreat;
        private System.Windows.Forms.Button button_Stop;
        private System.Windows.Forms.TrackBar trackBar_Power;
        private System.Windows.Forms.Button button_turn_left;
        private System.Windows.Forms.Button button_turn_right;
        private System.Windows.Forms.TrackBar trackBar_direction;
        private System.Windows.Forms.TextBox textBox_motor1;
        private System.Windows.Forms.TextBox textBox_motor2;
        private System.Windows.Forms.Button button_brake_state1;
        private System.Windows.Forms.Button button_brake_state2;
        private System.Windows.Forms.TextBox textBox_Remote2;
        private System.Windows.Forms.TextBox textBox_Remote4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label_power;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label_direction;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label21;
    }
}

