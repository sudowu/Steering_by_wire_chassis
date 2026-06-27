using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using control_software.Controls;

namespace control_software
{
    /// <summary>
    /// 线控底盘控制软件主界面类
    /// 提供CAN总线通信控制、底盘运动控制、状态监控等功能
    /// 
    /// ========== CAN通信协议说明 ==========
    /// 
    /// 1. 控制命令帧（ID=0x08，上位机→底盘）
    ///    数据格式：
    ///    - Data[0-1]: 速度值（16位，低字节+高字节）
    ///      * 前进/后退时有效
    ///      * 后退时速度为负值（通过取反实现）
    ///    - Data[2-3]: 角度值（16位，低字节+高字节）
    ///      * 左转/右转时有效
    ///      * 右转时角度为负值（通过取反实现）
    ///    - Data[4-7]: 保留字段（当前为0）
    /// 
    /// 2. 状态反馈帧（ID=0x09，底盘→上位机）
    ///    数据格式：
    ///    - Data[0-1]: 电机1转速（16位，低字节+高字节）
    ///    - Data[2-3]: 电机2转速（16位，低字节+高字节）
    ///    - Data[4-5]: 遥控器通道2数据（16位，前后控制）
    ///    - Data[6-7]: 遥控器通道4数据（16位，方向控制）
    /// 
    /// 3. 制动状态帧（ID=0x0A，底盘→上位机）
    ///    数据格式：
    ///    - Data[0]: 制动器1状态（1=制动，0=解除）
    ///    - Data[1]: 制动器2状态（1=制动，0=解除）
    /// 
    /// ========== 通信参数 ==========
    /// - 波特率：500Kbps（Timing0=0x00, Timing1=0x14）
    /// - 帧格式：标准帧、数据帧
    /// - 接收缓冲区：1000帧
    /// - 超时时间：100ms
    /// </summary>
    public partial class Form1 : Form
    {
        /// <summary>
        /// CAN设备控制对象
        /// </summary>
        controlcan controlcan = new controlcan();

        /// <summary>
        /// 设备类型，默认使用USBCAN2
        /// </summary>
        static UInt32 m_devtype = 4;//USBCAN2

        /// <summary>
        /// 设备打开标志：0-未打开，1-已打开
        /// </summary>
        UInt32 m_bOpen = 0;
        /// <summary>
        /// 设备索引
        /// </summary>
        UInt32 m_devind = 0;
        /// <summary>
        /// CAN通道索引
        /// </summary>
        UInt32 m_canind = 0;
        /// <summary>
        /// CAN通信启动标志：0-停止，1-启动
        /// </summary>
        UInt32 m_start = 0;
        /// <summary>
        /// CAN数据接收缓冲区，最多存储1000条消息
        /// </summary>
        VCI_CAN_OBJ[] m_recobj = new VCI_CAN_OBJ[1000];

        /// <summary>
        /// 设备类型数组，存储支持的设备类型
        /// </summary>
        UInt32[] m_arrdevtype = new UInt32[20];

        /// <summary>
        /// CAN设备初始化配置
        /// </summary>
        VCI_INIT_CONFIG config = new VCI_INIT_CONFIG();
        /// <summary>
        /// CAN发送数据对象
        /// </summary>
        VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
        /// <summary>
        /// 底盘信息结构体，包含运动状态、功率等信息
        /// </summary>
        CHASSIS_INFO cHASSIS_INFO = new CHASSIS_INFO();
        /// <summary>
        /// 构造函数，初始化窗体组件
        /// </summary>
        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 窗体加载事件处理方法
        /// 初始化底盘参数、CAN配置、UI控件默认值
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void Form1_Load(object sender, EventArgs e)
        {
            Int32 curindex = 0;
            // 初始化底盘参数
            cHASSIS_INFO.Max_Power = 0x0600;  // 设置最大功率为0x0600
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_IDLE;  // 设置初始状态为空闲
            //textBoxPower.Text = cHASSIS_INFO.Power.ToString();

            m_devind = 0;        // 设备索引
            m_canind = 0;        // CAN通道索引
            m_devtype = 4;
            m_start = 0;
            
            // 初始化CAN配置参数
            config.AccCode = 0;           // 验收码，全接收
            config.AccMask = 0xffffffff;  // 屏蔽码，全接收
            config.Timing0 = 0;           // 波特率定时器0
            config.Timing1 = 0x14;        // 波特率定时器1，设置波特率为500Kbps
            config.Filter = 1;            // 滤波方式
            config.Mode = 0;              // 工作模式：0-正常模式
            sendobj.ID = 0X08;            // 发送帧ID设置为0x08

            // 填充下拉列表项
            comboBox_DevIndex.Items.Clear();
            comboBox_DevIndex.Items.Add("0"); comboBox_DevIndex.Items.Add("1");
            comboBox_DevIndex.Items.Add("2"); comboBox_DevIndex.Items.Add("3");
            comboBox_DevIndex.SelectedIndex = (int)m_devind;

            comboBox_CANIndex.Items.Clear();
            comboBox_CANIndex.Items.Add("0"); comboBox_CANIndex.Items.Add("1");
            comboBox_CANIndex.SelectedIndex = (int)m_canind;

            comboBox_Filter.Items.Clear();
            comboBox_Filter.Items.Add("接收全部类型");
            comboBox_Filter.Items.Add("只接收标准帧");
            comboBox_Filter.Items.Add("只接收扩展帧");
            comboBox_Filter.SelectedIndex = config.Filter - 1;

            comboBox_Mode.Items.Clear();
            comboBox_Mode.Items.Add("正常");
            comboBox_Mode.Items.Add("只听");
            comboBox_Mode.Items.Add("自测");
            comboBox_Mode.SelectedIndex = config.Mode;

            comboBox_FrameFormat.Items.Clear();
            comboBox_FrameFormat.Items.Add("数据帧");
            comboBox_FrameFormat.Items.Add("远程帧");
            comboBox_FrameFormat.SelectedIndex = 0;

            comboBox_FrameType.Items.Clear();
            comboBox_FrameType.Items.Add("标准帧");
            comboBox_FrameType.Items.Add("扩展帧");
            comboBox_FrameType.SelectedIndex = 0;

            textBox_AccCode.Text = config.AccCode.ToString("X8");
            textBox_AccMask.Text = config.AccMask.ToString("X8");
            textBox_Time0.Text = config.Timing0.ToString("X2");
            textBox_Time1.Text = config.Timing1.ToString("X2");
            textBox_Data.Text = "00 00 00 00 00 00 00 00";
            textBox_ID.Text = sendobj.ID.ToString("X5");

            comboBox_devtype.Items.Clear();
            curindex = comboBox_devtype.Items.Add("DEV_USBCAN");
            m_arrdevtype[curindex] = controlcan.DEV_USBCAN;
            curindex = comboBox_devtype.Items.Add("DEV_USBCAN2");
            m_arrdevtype[curindex] = controlcan.DEV_USBCAN2;
            comboBox_devtype.SelectedIndex = curindex;

            
            listView_Info.Columns.Add("帧ID", 90, HorizontalAlignment.Center);
            listView_Info.Columns.Add("帧格式", 100, HorizontalAlignment.Center);
            listView_Info.Columns.Add("帧类型", 100, HorizontalAlignment.Center);
            listView_Info.Columns.Add("数据", 900, HorizontalAlignment.Left);
            listView_Info.SmallImageList = imageList1;

            // 应用现代暗色主题
            ApplyTheme();
        }

        /// <summary>
        /// 连接/断开CAN设备按钮事件处理方法
        /// 根据当前状态切换连接或断开CAN设备
        /// 
        /// 核心实现流程：
        /// 1. 如果设备已打开，则关闭设备
        /// 2. 如果设备未打开，则执行以下步骤：
        ///    - 获取设备类型、设备索引、CAN通道索引
        ///    - 调用VCI_OpenDevice打开设备
        ///    - 配置CAN参数（滤波方式、工作模式）
        ///    - 调用VCI_InitCAN初始化CAN通道
        /// 3. 更新按钮状态和启动定时器接收数据
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 1)
            {
                // 如果设备已打开，则关闭设备
                controlcan.VCI_CloseDevice(m_devtype, m_devind);
                m_bOpen = 0;
            }
            else
            {
                // 获取设备类型（从下拉框选择）
                m_devtype = m_arrdevtype[comboBox_devtype.SelectedIndex];
                // 获取设备索引（从下拉框选择，0表示第一个设备）
                m_devind = (UInt32)comboBox_DevIndex.SelectedIndex;
                // 获取CAN通道索引（从下拉框选择，0表示第一个通道）
                m_canind = (UInt32)comboBox_CANIndex.SelectedIndex;
                
                // 打开USBCAN2设备，返回0表示失败
                if (controlcan.VCI_OpenDevice(m_devtype, m_devind, 0) == 0)
                {
                    MessageBox.Show("打开设备失败,请检查设备类型和设备索引号是否正确", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }

                // 设备打开成功，设置标志
                m_bOpen = 1;
                m_start = 0;  // 重置启动标志
                
                // 配置CAN参数
                config.Filter = (Byte)(comboBox_Filter.SelectedIndex + 1);  // 滤波方式
                config.Mode = (Byte)comboBox_Mode.SelectedIndex;            // 工作模式（0=正常模式）
                
                // 初始化CAN通道，配置波特率、滤波等参数
                controlcan.VCI_InitCAN(m_devtype, m_devind, m_canind, ref config);
            }
            // 更新按钮文本：已打开显示"断开"，未打开显示"连接"
            buttonConnect.Text = m_bOpen == 1 ? "断开设备" : "连接设备";
            // 更新状态栏
            if (m_bOpen == 1)
            {
                labelStatusConn.Text = "●  已连接";
                labelStatusConn.ForeColor = ThemeColors.StatusOnline;
                labelStatusDev.Text = comboBox_devtype.SelectedItem.ToString();
                labelStatusCh.Text = "通道 " + m_canind.ToString();
            }
            else
            {
                labelStatusConn.Text = "●  未连接";
                labelStatusConn.ForeColor = ThemeColors.StatusOffline;
                labelStatusCAN.Text = "CAN 停止";
                labelStatusCAN.ForeColor = ThemeColors.StatusOffline;
                labelStatusDev.Text = "—";
                labelStatusCh.Text = "通道 —";
            }
            // 设备打开后启动定时器，开始接收CAN数据
            timer_rec.Enabled = m_bOpen == 1 ? true : false;
        }

        /// <summary>
        /// 定时器事件处理方法，周期性执行CAN通信任务
        /// 
        /// 主要功能：
        /// 1. 接收CAN总线上的数据帧（定时器周期性调用，实现实时通信）
        /// 2. 解析并显示接收到的消息（更新信息列表）
        /// 3. 根据底盘状态发送控制命令（周期性发送，保证控制连续性）
        /// 4. 更新UI显示状态信息（电机、制动、遥控数据）
        /// 
        /// 核心机制：
        /// - 定时器周期性执行，实现实时双向通信
        /// - 接收缓冲区最多1000帧，避免丢帧
        /// - 超时时间100ms，平衡实时性和性能
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        unsafe private void timer_rec_Tick(object sender, EventArgs e)
        {
            UInt32 res = new UInt32();

            // 接收CAN数据
            // 参数说明：
            // - m_devtype, m_devind, m_canind: 设备类型、设备索引、CAN通道索引
            // - m_recobj[0]: 接收缓冲区起始地址（最多存储1000帧）
            // - 1000: 最大接收帧数
            // - 100: 超时时间（毫秒）
            // 返回值：实际接收到的帧数
            res = controlcan.VCI_Receive(m_devtype, m_devind, m_canind, ref m_recobj[0], 1000, 100);

            // 当设备未初始化时，返回0xFFFFFFFF，设置为0以避免显示错误
            if (res == 0xFFFFFFFF) res = 0;
            
            
            // 遍历所有接收到的CAN帧
            for (UInt32 i = 0; i < res; i++)
            {
                // 创建列表视图项用于显示CAN消息
                ListViewItem lvi = new ListViewItem();

                String str = "";
                lvi.ImageIndex = 1;  // 设置图标索引
                lvi.Text = "0x" + m_recobj[i].ID.ToString("X2");  // 显示帧ID

                // 判断并显示帧类型
                if (m_recobj[i].RemoteFlag == 0)
                    lvi.SubItems.Add("数据帧");
                else
                    lvi.SubItems.Add("远程帧");

                // 判断并显示帧格式
                if (m_recobj[i].ExternFlag == 0)
                    lvi.SubItems.Add("标准帧");
                else
                    lvi.SubItems.Add("扩展帧");
                //////////////////////////////////////////
                // 如果是数据帧，解析数据内容
                if (m_recobj[i].RemoteFlag == 0)
                {
                    byte len = (byte)(m_recobj[i].DataLen % 9);  // 获取数据长度
                    byte j = 0;
                    fixed (VCI_CAN_OBJ* m_recobj1 = &m_recobj[i])
                    {
                        // 格式化显示数据字节（十六进制）
                        if (j++ < len)
                            str += " " + m_recobj1->Data[0].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[1].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[2].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[3].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[4].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[5].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[6].ToString("X2");
                        if (j++ < len)
                            str += " " + m_recobj1->Data[7].ToString("X2");

                        // ========== 数据解析：根据帧ID解析不同类型的数据 ==========
                        
                        // 特殊处理ID为0x0A的制动状态帧（底盘→上位机）
                        // 数据格式：
                        // - Data[0]: 制动器1状态（1=制动，0=解除）
                        // - Data[1]: 制动器2状态（1=制动，0=解除）
                        if(m_recobj1->ID == 0x0A)
                        {
                            // 根据Data[0]更新制动器1状态
                            if(m_recobj1->Data[0] == 1)
                            {
                                button_brake_state1.UseAccent = true;
                                button_brake_state1.AccentColor = ThemeColors.Danger;
                                button_brake_state1.Text = "制动";
                            }
                            else
                            {
                                button_brake_state1.UseAccent = true;
                                button_brake_state1.AccentColor = ThemeColors.Success;
                                button_brake_state1.Text = "解除";
                            }

                            // 根据Data[1]更新制动器2状态
                            if(m_recobj1->Data[1] == 1)
                            {
                                button_brake_state2.UseAccent = true;
                                button_brake_state2.AccentColor = ThemeColors.Danger;
                                button_brake_state2.Text = "制动";
                            }
                            else
                            {
                                button_brake_state2.UseAccent = true;
                                button_brake_state2.AccentColor = ThemeColors.Success;
                                button_brake_state2.Text = "解除";
                            }
                        }
                        
                        // 特殊处理ID为0x09的电机和遥控器数据帧（底盘→上位机）
                        // 数据格式：
                        // - Data[0-1]: 电机1转速（16位，低字节+高字节）
                        // - Data[2-3]: 电机2转速（16位，低字节+高字节）
                        // - Data[4-5]: 遥控器通道2数据（16位，前后控制）
                        // - Data[6-7]: 遥控器通道4数据（16位，方向控制）
                        if(m_recobj1->ID == 0x09)
                        {
                            // 解析电机1转速：Data[0]低字节 | (Data[1]高字节 << 8)
                            // 示例：Data[0]=0x34, Data[1]=0x12 → 0x1234 = 4660
                            textBox_motor1.Text = (m_recobj1->Data[0] | (m_recobj1 -> Data[1] << 8)).ToString();
                            
                            // 解析电机2转速：Data[2]低字节 | (Data[3]高字节 << 8)
                            textBox_motor2.Text = (m_recobj1->Data[2] | (m_recobj1->Data[3] << 8)).ToString();
                            
                            // 解析遥控器通道2数据：前后控制通道
                            textBox_Remote2.Text = (m_recobj1->Data[4] | (m_recobj1->Data[5] << 8)).ToString();
                            
                            // 解析遥控器通道4数据：方向控制通道
                            textBox_Remote4.Text = (m_recobj1->Data[6] | (m_recobj1->Data[7] << 8)).ToString();
                        }
                    }
                }
                this.listView_Info.BeginUpdate();   //数据更新，UI暂时挂起，直到EndUpdate绘制控件，可以有效避免闪烁并大大提高加载速度 
                lvi.SubItems.Add(str);
                listView_Info.Items.Add(lvi);
                listView_Info.EndUpdate();  //结束数据处理，UI界面一次性绘制。
                if(listView_Info.Items.Count > 0)
                    listView_Info.EnsureVisible(listView_Info.Items.Count - 1);
            }
            // ========== 控制命令发送：根据底盘状态周期性发送控制命令 ==========
            
            // 前置条件检查：如果设备未打开、处于空闲状态或CAN未启动，则不发送控制命令
            if ((m_bOpen == 0 || cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_IDLE) || m_start == 0 )
                return;

            // 创建发送对象
            VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
            sendobj.RemoteFlag = 0;    // 数据帧（0=数据帧，1=远程帧）
            sendobj.ExternFlag = 0;    // 标准帧（0=标准帧，1=扩展帧）
            sendobj.ID = 0x08;         // 控制命令帧ID（上位机→底盘）
            sendobj.DataLen = 8;       // 数据长度8字节
            
            // 根据滑块值计算实际速度和角度
            // 公式：实际值 = 最大功率 / 100 * 滑块百分比值
            // 示例：Max_Power=0x0600(1536), 滑块=50% → speed = 1536/100*50 = 768
            UInt16 speed = (UInt16)(cHASSIS_INFO.Max_Power / 100 * trackBar_Power.Value);
            UInt16 angle = (UInt16)(cHASSIS_INFO.Max_Power / 100 * trackBar_direction.Value);

            // 创建发送消息的列表显示项
            ListViewItem lvi2 = new ListViewItem();
            lvi2.Text = "0x08";           // 显示发送帧ID
            lvi2.ImageIndex = 0;          // 设置图标
            lvi2.SubItems.Add("标准帧");    // 帧格式
            lvi2.SubItems.Add("数据帧");    // 帧类型
            

            // 根据运动状态调整速度和角度方向
            // 后退时速度为负值（通过取反实现）
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RETREAT)
                speed = (UInt16)(0 - speed);  // 后退时速度为负
            // 右转时角度为负值（通过取反实现）
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RIGHT)
                angle = (UInt16)(0 - angle);  // 右转时角度为负

            // 处理停止命令（紧急停止功能）
            if (cHASSIS_INFO.stop_flag == 1)
            {
                speed = 0;  // 速度清零，立即停止
                cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_IDLE;  // 状态改为闲置
                cHASSIS_INFO.stop_flag = 0;  // 清除停止标志，避免重复处理
            }
            
            // ========== 数据组装：将速度和角度数据拆分并组装到CAN帧中 ==========
            
            // 初始化数据缓冲区（全部清零）
            sendobj.Data[0] = 0;
            sendobj.Data[1] = 0;

            // 如果是前进或后退状态，设置速度数据
            // 数据格式：Data[0-1] = 速度值（16位，低字节+高字节）
            // 示例：speed=0x1234 → Data[0]=0x34, Data[1]=0x12
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RETREAT || 
                cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RUNNING)
            {
                sendobj.Data[0] = System.Convert.ToByte(speed & 0xff);   // 速度低字节（取低8位）
                sendobj.Data[1] = System.Convert.ToByte(speed >> 8);       // 速度高字节（右移8位取高8位）
            }

            sendobj.Data[2] = 0;
            sendobj.Data[3] = 0;

            // 如果是左转或右转状态，设置角度数据
            // 数据格式：Data[2-3] = 角度值（16位，低字节+高字节）
            // 示例：angle=0x5678 → Data[2]=0x78, Data[3]=0x56
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_LEFT || 
                cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RIGHT)
            {
                sendobj.Data[2] = (byte)(angle & 0x00ff);  // 角度低字节（取低8位）
                sendobj.Data[3] = (byte)(angle >> 8);      // 角度高字节（右移8位取高8位）
            }

            // 保留字段（当前未使用，全部为0）
            sendobj.Data[4] = 0;
            sendobj.Data[5] = 0;
            sendobj.Data[6] = 0;
            sendobj.Data[7] = 0;
            
            // 格式化显示数据（用于信息列表显示）
            String strdata = sendobj.Data[0].ToString("X2") +" "+ sendobj.Data[1].ToString("X2")
                + " 00 00 00 00 00 00";
            lvi2.SubItems.Add(strdata);
            
            // 更新信息列表显示（显示发送的帧）
            listView_Info.BeginUpdate();
            listView_Info.Items.Add(lvi2);
            listView_Info.EndUpdate();
            listView_Info.EnsureVisible(listView_Info.Items.Count - 1);
            
            // 发送控制命令到CAN总线
            // 参数说明：
            // - m_devtype, m_devind, m_canind: 设备类型、设备索引、CAN通道索引
            // - sendobj: 要发送的CAN对象
            // - 1: 发送帧数
            // 返回值：0表示失败，非0表示成功
            if (controlcan.VCI_Transmit(m_devtype, m_devind, m_canind, ref sendobj, 1) == 0)
            {
                MessageBox.Show("发送失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            //Marshal.FreeHGlobal(ptArray[0]);
            //Marshal.FreeHGlobal(pt);
        }

        /// <summary>
        /// 启动CAN通信按钮事件处理方法
        /// 启动CAN总线通信，允许接收和发送数据
        /// 
        /// 核心功能：
        /// 1. 检查设备是否已打开
        /// 2. 调用VCI_StartCAN启动CAN通道
        /// 3. 设置启动标志，允许定时器发送控制命令
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_StartCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)  // 检查设备是否已打开
                return;
            // 启动CAN通道，使能接收和发送功能
            controlcan.VCI_StartCAN(m_devtype, m_devind, m_canind);
            m_start = 1;  // 设置启动标志，定时器可以开始发送控制命令
            labelStatusCAN.Text = "●  CAN 运行中";
            labelStatusCAN.ForeColor = ThemeColors.StatusOnline;
        }

        /// <summary>
        /// 停止CAN通信按钮事件处理方法
        /// 停止CAN总线通信，重置CAN通道状态
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_StopCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            controlcan.VCI_ResetCAN(m_devtype, m_devind, m_canind);
            m_start = 0;
            labelStatusCAN.Text = "CAN 停止";
            labelStatusCAN.ForeColor = ThemeColors.StatusOffline;
        }

        /// <summary>
        /// 清空消息列表按钮事件处理方法
        /// 清空CAN消息显示列表
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_Clear_Click(object sender, EventArgs e)
        {
            listView_Info.Items.Clear();
        }

        /// <summary>
        /// 发送CAN消息按钮事件处理方法
        /// 根据用户输入的ID和数据发送自定义CAN消息
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        unsafe private void button_Send_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            ListViewItem lvi = new ListViewItem();
            VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
            //sendobj.Init();
            sendobj.RemoteFlag = (byte)comboBox_FrameFormat.SelectedIndex;
            sendobj.ExternFlag = (byte)comboBox_FrameType.SelectedIndex;
            sendobj.ID = System.Convert.ToUInt32("0x" + textBox_ID.Text, 16);
            int len = (textBox_Data.Text.Length + 1) / 3;
            sendobj.DataLen = System.Convert.ToByte(len);
            String strdata = textBox_Data.Text;

            lvi.Text = textBox_ID.Text;
            lvi.ImageIndex = 0;
            lvi.SubItems.Add(sendobj.ExternFlag == 1 ? "扩展帧" : "标准帧");
            lvi.SubItems.Add(sendobj.RemoteFlag == 1 ? "远程帧" : "数据帧");
            lvi.SubItems.Add(strdata);

            int i = -1;
            if (i++ < len - 1)
                sendobj.Data[0] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[1] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[2] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[3] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[4] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[5] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[6] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[7] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

            listView_Info.BeginUpdate();
            listView_Info.Items.Add(lvi);
            listView_Info.EndUpdate();

            if (controlcan.VCI_Transmit(m_devtype, m_devind, m_canind, ref sendobj, 1) == 0)
            {
                MessageBox.Show("发送失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            
        }

        /// <summary>
        /// 前进按钮事件处理方法
        /// 设置底盘状态为前进，并更新UI按钮状态
        /// 
        /// 核心功能：
        /// 1. 检查设备是否已打开
        /// 2. 设置底盘状态为CHASSIS_STATUS_RUNNING（前进状态）
        /// 3. 更新UI按钮颜色：前进按钮高亮（绿色），其他按钮恢复默认
        /// 4. 定时器会根据此状态发送前进控制命令
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_advance_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)  // 检查设备是否已打开
                return;
            // 设置底盘状态为前进状态
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RUNNING;
            // 更新UI按钮状态：前进按钮高亮，其他按钮恢复默认
            button_advance.UseAccent = true;
            button_advance.AccentColor = ThemeColors.Success;
            button_Retreat.UseAccent = false;
            button_turn_right.UseAccent = false;
            button_turn_left.UseAccent = false;
        }

        /// <summary>
        /// 后退按钮事件处理方法
        /// 设置底盘状态为后退，并更新UI按钮状态
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_Retreat_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RETREAT;
            button_Retreat.UseAccent = true;
            button_Retreat.AccentColor = ThemeColors.Warning;
            button_advance.UseAccent = false;
            button_turn_right.UseAccent = false;
            button_turn_left.UseAccent = false;
        }

        /// <summary>
        /// 停止按钮事件处理方法
        /// 设置停止标志，停止底盘运动，并重置所有运动按钮状态
        /// 
        /// 核心功能：
        /// 1. 检查设备是否已打开
        /// 2. 设置停止标志stop_flag=1
        /// 3. 重置所有运动按钮颜色为默认
        /// 4. 定时器检测到停止标志后会将速度清零并恢复空闲状态
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_Stop_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)  // 检查设备是否已打开
                return;
            // 设置停止标志，定时器会在下次循环中处理此标志
            cHASSIS_INFO.stop_flag = 1;
            // 重置所有运动按钮为默认
            button_turn_right.UseAccent = false;
            button_turn_left.UseAccent = false;
            button_advance.UseAccent = false;
            button_Retreat.UseAccent = false;
        }

        /// <summary>
        /// 左转按钮事件处理方法
        /// 设置底盘状态为左转，并更新UI按钮状态
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_turn_left_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_LEFT;
            button_turn_left.UseAccent = true;
            button_turn_left.AccentColor = ThemeColors.Info;
            button_advance.UseAccent = false;
            button_Retreat.UseAccent = false;
            button_turn_right.UseAccent = false;
        }

        /// <summary>
        /// 右转按钮事件处理方法
        /// 设置底盘状态为右转，并更新UI按钮状态
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void button_turn_right_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RIGHT;
            button_turn_right.UseAccent = true;
            button_turn_right.AccentColor = ThemeColors.Info;
            button_turn_left.UseAccent = false;
            button_advance.UseAccent = false;
            button_Retreat.UseAccent = false;
        }

        private void comboBox_CANIndex_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// 方向控制滑块滚动事件处理方法
        /// 更新方向控制标签显示当前百分比值
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void trackBar_direction_Scroll(object sender, EventArgs e)
        {
            label_direction.Text = trackBar_direction.Value.ToString() + "%";
        }

        /// <summary>
        /// 功率控制滑块滚动事件处理方法
        /// 更新功率控制标签显示当前百分比值
        /// </summary>
        /// <param name="sender">事件发送者</param>
        /// <param name="e">事件参数</param>
        private void trackBar_Power_Scroll(object sender, EventArgs e)
        {
            label_power.Text = trackBar_Power.Value.ToString() + "%";
        }

        // Windows 深色标题栏 API
        [DllImport("dwmapi.dll")]
        private static extern int DwmSetWindowAttribute(IntPtr hwnd, int attr, ref int attrValue, int attrSize);

        private const int DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
        private const int DWMWA_BORDER_COLOR = 34;
        private const int DWMWA_CAPTION_COLOR = 35;

        /// <summary>
        /// 启用 Windows 深色标题栏（在 Handle 创建后调用）
        /// </summary>
        private void SetDarkTitleBar()
        {
            if (!this.IsHandleCreated) return;

            int useDarkMode = 1;
            DwmSetWindowAttribute(this.Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, ref useDarkMode, sizeof(int));

            // Windows 11: 同时设置标题栏和边框颜色
            int darkColor = ThemeColors.Background.ToArgb() & 0xFFFFFF;
            DwmSetWindowAttribute(this.Handle, DWMWA_CAPTION_COLOR, ref darkColor, sizeof(int));
            DwmSetWindowAttribute(this.Handle, DWMWA_BORDER_COLOR, ref darkColor, sizeof(int));
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            SetDarkTitleBar();
        }

        /// <summary>
        /// 应用现代暗色主题到所有控件
        /// </summary>
        private void ApplyTheme()
        {
            // 窗体背景
            this.BackColor = ThemeColors.Background;
            this.ForeColor = ThemeColors.TextPrimary;

            // 递归设置所有控件主题
            ApplyThemeToControls(this.Controls);

            // 设置 ListView 为 OwnerDraw 模式
            listView_Info.OwnerDraw = true;
            listView_Info.BackColor = ThemeColors.Surface;
            listView_Info.ForeColor = ThemeColors.TextPrimary;
            listView_Info.DrawColumnHeader += ListView_Info_DrawColumnHeader;
            listView_Info.DrawItem += ListView_Info_DrawItem;
            listView_Info.DrawSubItem += ListView_Info_DrawSubItem;

            // GroupBox 特殊处理
            foreach (Control ctrl in this.Controls)
            {
                if (ctrl is ModernGroupBox gb)
                {
                    gb.BackColor = ThemeColors.Surface;
                    gb.ForeColor = ThemeColors.Accent;
                }
            }
        }

        /// <summary>
        /// 递归应用主题到控件树
        /// </summary>
        private void ApplyThemeToControls(Control.ControlCollection controls)
        {
            foreach (Control ctrl in controls)
            {
                if (ctrl is ModernButton btn)
                {
                    btn.BackColor = ThemeColors.ButtonNormal;
                    btn.ForeColor = ThemeColors.TextPrimary;
                }
                else if (ctrl is ModernGroupBox)
                {
                    // GroupBox 已在 ApplyTheme 中处理
                }
                else if (ctrl is ModernComboBox mcb)
                {
                    mcb.BackColor = ThemeColors.SurfaceLight;
                    mcb.ForeColor = ThemeColors.TextPrimary;
                }
                else if (ctrl is TextBox tb)
                {
                    tb.BackColor = ThemeColors.SurfaceLight;
                    tb.ForeColor = ThemeColors.TextPrimary;
                    tb.BorderStyle = BorderStyle.FixedSingle;
                }
                else if (ctrl is Label lbl)
                {
                    // 保留状态栏标签的显式颜色
                    if (lbl.Name.StartsWith("labelStatus")) { }
                    else
                    {
                        lbl.ForeColor = ThemeColors.TextPrimary;
                    }
                    lbl.BackColor = Color.Transparent;
                }
                else if (ctrl is TrackBar tb2)
                {
                    tb2.BackColor = ThemeColors.PanelBg;
                }
                else if (ctrl is ListView lv)
                {
                    lv.BackColor = ThemeColors.PanelBg;
                    lv.ForeColor = ThemeColors.TextPrimary;
                }
                else if (ctrl is Panel pnl)
                {
                    pnl.BackColor = ThemeColors.PanelBg;
                    pnl.ForeColor = ThemeColors.TextPrimary;
                }
                else
                {
                    if (!(ctrl is Label lbl2 && lbl2.Name.StartsWith("labelStatus")))
                    {
                        ctrl.BackColor = ThemeColors.PanelBg;
                    }
                    ctrl.ForeColor = ThemeColors.TextPrimary;
                }

                // 递归子控件
                if (ctrl.HasChildren)
                    ApplyThemeToControls(ctrl.Controls);
            }
        }

        /// <summary>
        /// ListView 列标题自绘 — 暗色主题
        /// </summary>
        private void ListView_Info_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            e.DrawDefault = false;
            using (var bgBrush = new SolidBrush(ThemeColors.SurfaceLight))
            using (var textBrush = new SolidBrush(ThemeColors.TextPrimary))
            using (var pen = new Pen(ThemeColors.Border))
            {
                e.Graphics.FillRectangle(bgBrush, e.Bounds);
                e.Graphics.DrawLine(pen, e.Bounds.Left, e.Bounds.Bottom - 1,
                                    e.Bounds.Right, e.Bounds.Bottom - 1);

                var textRect = new Rectangle(e.Bounds.X + 4, e.Bounds.Y + 2,
                    e.Bounds.Width - 8, e.Bounds.Height - 4);
                e.Graphics.DrawString(e.Header.Text, this.Font, textBrush, textRect);
            }
        }

        /// <summary>
        /// ListView 行自绘 — 暗色主题
        /// </summary>
        private void ListView_Info_DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            e.DrawDefault = false;
            Color bgColor;
            if ((e.State & ListViewItemStates.Selected) != 0)
                bgColor = ThemeColors.Accent;
            else if (e.ItemIndex % 2 == 0)
                bgColor = ThemeColors.Surface;
            else
                bgColor = ThemeColors.SurfaceLight;

            using (var bgBrush = new SolidBrush(bgColor))
            {
                e.Graphics.FillRectangle(bgBrush, e.Bounds);
            }

            // 绘制图标（发送/接收指示灯）
            if (e.Item.ImageIndex >= 0 && imageList1.Images.Count > e.Item.ImageIndex)
            {
                var img = imageList1.Images[e.Item.ImageIndex];
                int y = e.Bounds.Y + (e.Bounds.Height - img.Height) / 2;
                e.Graphics.DrawImage(img, e.Bounds.X + 4, y, img.Width, img.Height);
            }
        }

        /// <summary>
        /// ListView 单元格自绘 — 暗色主题
        /// </summary>
        private void ListView_Info_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            e.DrawDefault = false;
            Color bgColor;
            if ((e.ItemState & ListViewItemStates.Selected) != 0)
                bgColor = ThemeColors.Accent;
            else if (e.ItemIndex % 2 == 0)
                bgColor = ThemeColors.Surface;
            else
                bgColor = ThemeColors.SurfaceLight;

            using (var bgBrush = new SolidBrush(bgColor))
            {
                e.Graphics.FillRectangle(bgBrush, e.Bounds);
            }

            // 文字偏移（第一列给图标留空间）
            int textOffset = e.ColumnIndex == 0 ? 24 : 4;
            var textRect = new Rectangle(e.Bounds.X + textOffset, e.Bounds.Y + 2,
                e.Bounds.Width - textOffset - 4, e.Bounds.Height - 4);

            Color textColor;
            if ((e.ItemState & ListViewItemStates.Selected) != 0)
                textColor = Color.White;
            else
                textColor = ThemeColors.TextPrimary;

            using (var textBrush = new SolidBrush(textColor))
            using (var sf = new StringFormat { LineAlignment = StringAlignment.Center })
            {
                e.Graphics.DrawString(e.SubItem.Text, this.Font, textBrush, textRect, sf);
            }
        }


    }
}
