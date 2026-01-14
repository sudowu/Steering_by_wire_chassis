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
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(46, 49);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(101, 46);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "连接";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // button_StartCAN
            // 
            this.button_StartCAN.Location = new System.Drawing.Point(167, 49);
            this.button_StartCAN.Name = "button_StartCAN";
            this.button_StartCAN.Size = new System.Drawing.Size(101, 46);
            this.button_StartCAN.TabIndex = 0;
            this.button_StartCAN.Text = "启动CAN";
            this.button_StartCAN.UseVisualStyleBackColor = true;
            this.button_StartCAN.Click += new System.EventHandler(this.button_StartCAN_Click);
            // 
            // button_StopCAN
            // 
            this.button_StopCAN.Location = new System.Drawing.Point(289, 49);
            this.button_StopCAN.Name = "button_StopCAN";
            this.button_StopCAN.Size = new System.Drawing.Size(101, 46);
            this.button_StopCAN.TabIndex = 0;
            this.button_StopCAN.Text = "复位CAN";
            this.button_StopCAN.UseVisualStyleBackColor = true;
            this.button_StopCAN.Click += new System.EventHandler(this.button_StopCAN_Click);
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(417, 49);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(101, 46);
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
            this.comboBox_devtype.Location = new System.Drawing.Point(46, 125);
            this.comboBox_devtype.Name = "comboBox_devtype";
            this.comboBox_devtype.Size = new System.Drawing.Size(187, 26);
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
            this.comboBox_DevIndex.Location = new System.Drawing.Point(288, 125);
            this.comboBox_DevIndex.Name = "comboBox_DevIndex";
            this.comboBox_DevIndex.Size = new System.Drawing.Size(65, 26);
            this.comboBox_DevIndex.TabIndex = 3;
            this.comboBox_DevIndex.Text = "索引号";
            // 
            // comboBox_CANIndex
            // 
            this.comboBox_CANIndex.FormattingEnabled = true;
            this.comboBox_CANIndex.Items.AddRange(new object[] {
            "0",
            "1"});
            this.comboBox_CANIndex.Location = new System.Drawing.Point(388, 125);
            this.comboBox_CANIndex.Name = "comboBox_CANIndex";
            this.comboBox_CANIndex.Size = new System.Drawing.Size(121, 26);
            this.comboBox_CANIndex.TabIndex = 4;
            this.comboBox_CANIndex.Text = "通道";
            // 
            // comboBox_Filter
            // 
            this.comboBox_Filter.FormattingEnabled = true;
            this.comboBox_Filter.Items.AddRange(new object[] {
            "接收全部类型",
            "只接收标准帧",
            "只接收扩展帧"});
            this.comboBox_Filter.Location = new System.Drawing.Point(339, 240);
            this.comboBox_Filter.Name = "comboBox_Filter";
            this.comboBox_Filter.Size = new System.Drawing.Size(121, 26);
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
            this.comboBox_Mode.Location = new System.Drawing.Point(339, 188);
            this.comboBox_Mode.Name = "comboBox_Mode";
            this.comboBox_Mode.Size = new System.Drawing.Size(121, 26);
            this.comboBox_Mode.TabIndex = 4;
            this.comboBox_Mode.Text = "模式";
            // 
            // comboBox_FrameType
            // 
            this.comboBox_FrameType.FormattingEnabled = true;
            this.comboBox_FrameType.Items.AddRange(new object[] {
            "标准帧",
            "扩展帧"});
            this.comboBox_FrameType.Location = new System.Drawing.Point(205, 338);
            this.comboBox_FrameType.Name = "comboBox_FrameType";
            this.comboBox_FrameType.Size = new System.Drawing.Size(121, 26);
            this.comboBox_FrameType.TabIndex = 4;
            this.comboBox_FrameType.Text = "帧类型";
            // 
            // comboBox_FrameFormat
            // 
            this.comboBox_FrameFormat.FormattingEnabled = true;
            this.comboBox_FrameFormat.Items.AddRange(new object[] {
            "数据帧",
            "远程帧"});
            this.comboBox_FrameFormat.Location = new System.Drawing.Point(46, 336);
            this.comboBox_FrameFormat.Name = "comboBox_FrameFormat";
            this.comboBox_FrameFormat.Size = new System.Drawing.Size(121, 26);
            this.comboBox_FrameFormat.TabIndex = 4;
            this.comboBox_FrameFormat.Text = "帧格式";
            // 
            // textBox_Data
            // 
            this.textBox_Data.Location = new System.Drawing.Point(46, 401);
            this.textBox_Data.Name = "textBox_Data";
            this.textBox_Data.Size = new System.Drawing.Size(280, 28);
            this.textBox_Data.TabIndex = 5;
            // 
            // button_Send
            // 
            this.button_Send.Location = new System.Drawing.Point(396, 394);
            this.button_Send.Name = "button_Send";
            this.button_Send.Size = new System.Drawing.Size(122, 39);
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
            this.textBox_ID.Location = new System.Drawing.Point(416, 336);
            this.textBox_ID.Name = "textBox_ID";
            this.textBox_ID.Size = new System.Drawing.Size(100, 28);
            this.textBox_ID.TabIndex = 8;
            this.textBox_ID.Text = "帧ID";
            // 
            // textBox_Time0
            // 
            this.textBox_Time0.Location = new System.Drawing.Point(204, 188);
            this.textBox_Time0.Name = "textBox_Time0";
            this.textBox_Time0.Size = new System.Drawing.Size(100, 28);
            this.textBox_Time0.TabIndex = 8;
            this.textBox_Time0.Text = "定时器0";
            // 
            // textBox_Time1
            // 
            this.textBox_Time1.Location = new System.Drawing.Point(204, 240);
            this.textBox_Time1.Name = "textBox_Time1";
            this.textBox_Time1.Size = new System.Drawing.Size(100, 28);
            this.textBox_Time1.TabIndex = 9;
            this.textBox_Time1.Text = "定时器1";
            // 
            // textBox_AccCode
            // 
            this.textBox_AccCode.Location = new System.Drawing.Point(46, 186);
            this.textBox_AccCode.Name = "textBox_AccCode";
            this.textBox_AccCode.Size = new System.Drawing.Size(100, 28);
            this.textBox_AccCode.TabIndex = 8;
            this.textBox_AccCode.Text = "验收码";
            // 
            // textBox_AccMask
            // 
            this.textBox_AccMask.Location = new System.Drawing.Point(46, 238);
            this.textBox_AccMask.Name = "textBox_AccMask";
            this.textBox_AccMask.Size = new System.Drawing.Size(100, 28);
            this.textBox_AccMask.TabIndex = 9;
            this.textBox_AccMask.Text = "屏蔽码";
            // 
            // listView_Info
            // 
            this.listView_Info.HideSelection = false;
            this.listView_Info.Location = new System.Drawing.Point(46, 472);
            this.listView_Info.Name = "listView_Info";
            this.listView_Info.Size = new System.Drawing.Size(540, 380);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1219, 897);
            this.Controls.Add(this.listView_Info);
            this.Controls.Add(this.textBox_AccMask);
            this.Controls.Add(this.textBox_Time1);
            this.Controls.Add(this.textBox_AccCode);
            this.Controls.Add(this.textBox_Time0);
            this.Controls.Add(this.textBox_ID);
            this.Controls.Add(this.button_Send);
            this.Controls.Add(this.textBox_Data);
            this.Controls.Add(this.comboBox_Mode);
            this.Controls.Add(this.comboBox_Filter);
            this.Controls.Add(this.comboBox_FrameFormat);
            this.Controls.Add(this.comboBox_FrameType);
            this.Controls.Add(this.comboBox_CANIndex);
            this.Controls.Add(this.comboBox_DevIndex);
            this.Controls.Add(this.comboBox_devtype);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.button_StopCAN);
            this.Controls.Add(this.button_StartCAN);
            this.Controls.Add(this.buttonConnect);
            this.Name = "Form1";
            this.Text = "线控底盘检测控制系统";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

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
    }
}

