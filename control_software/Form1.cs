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
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace control_software
{

    public partial class Form1 : Form
    {
        controlcan controlcan = new controlcan();

        

        static UInt32 m_devtype = 4;//USBCAN2
        
        UInt32 m_bOpen = 0;         // 设备打开标志：0-未打开，1-已打开
        UInt32 m_devind = 0;        // 设备索引
        UInt32 m_canind = 0;        // CAN通道索引
        UInt32 m_start = 0;
        VCI_CAN_OBJ[] m_recobj = new VCI_CAN_OBJ[1000];

        UInt32[] m_arrdevtype = new UInt32[20];

        VCI_INIT_CONFIG config = new VCI_INIT_CONFIG();
        VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
        CHASSIS_INFO cHASSIS_INFO = new CHASSIS_INFO();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Int32 curindex = 0;
            //最大功率设置
            cHASSIS_INFO.Max_Power = 0x0600;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_IDLE;
            //textBoxPower.Text = cHASSIS_INFO.Power.ToString();

            m_devind = 0;        // 设备索引
            m_canind = 0;        // CAN通道索引
            m_devtype = 4;
            m_start = 0;
            
            config.AccCode = 0;
            config.AccMask = 0xffffffff;
            config.Timing0 = 0;
            config.Timing1 = 0x14;
            config.Filter = 1;
            config.Mode = 0;
            sendobj.ID = 0X08;

            comboBox_DevIndex.SelectedIndex = (int)m_devind;
            comboBox_CANIndex.SelectedIndex = (int)m_canind;
            comboBox_Filter.SelectedIndex = config.Filter - 1;
            comboBox_Mode.SelectedIndex = config.Mode;

            textBox_AccCode.Text = config.AccCode.ToString("X8");
            textBox_AccMask.Text = config.AccMask.ToString("X8");
            textBox_Time0.Text = config.Timing0.ToString("X2");
            textBox_Time1.Text = config.Timing1.ToString("X2");
            comboBox_FrameType.SelectedIndex = 0;
            comboBox_FrameFormat.SelectedIndex = 0;
            textBox_Data.Text = "00 00 00 00 00 00 00 00";
            textBox_ID.Text = sendobj.ID.ToString("X5");

            comboBox_devtype.Items.Clear();
            curindex = comboBox_devtype.Items.Add("DEV_USBCAN");
            m_arrdevtype[curindex] = controlcan.DEV_USBCAN;
            curindex = comboBox_devtype.Items.Add("DEV_USBCAN2");
            m_arrdevtype[curindex] = controlcan.DEV_USBCAN2;
            comboBox_devtype.SelectedIndex = curindex;

            
            listView_Info.Columns.Add("帧ID", 70, HorizontalAlignment.Center);
            listView_Info.Columns.Add("帧格式", 60, HorizontalAlignment.Center);
            listView_Info.Columns.Add("帧类型", 60, HorizontalAlignment.Center);
            listView_Info.Columns.Add("数据", 200, HorizontalAlignment.Center);
            listView_Info.SmallImageList = imageList1;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 1)
            {
                controlcan.VCI_CloseDevice(m_devtype, m_devind);
                m_bOpen = 0;
            }
            else
            {
                m_devtype = m_arrdevtype[comboBox_devtype.SelectedIndex];

                m_devind = (UInt32)comboBox_DevIndex.SelectedIndex;
                m_canind = (UInt32)comboBox_CANIndex.SelectedIndex;
                if (controlcan.VCI_OpenDevice(m_devtype, m_devind, 0) == 0)
                {
                    MessageBox.Show("打开设备失败,请检查设备类型和设备索引号是否正确", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }

                m_bOpen = 1;
                m_start = 0;
                //VCI_INIT_CONFIG config = new VCI_INIT_CONFIG();
                //config.AccCode = System.Convert.ToUInt32("0x" + textBox_AccCode.Text, 16);
                //config.AccMask = System.Convert.ToUInt32("0x" + textBox_AccMask.Text, 16);
                //config.Timing0 = System.Convert.ToByte("0x" + textBox_Time0.Text, 16);
                //config.Timing1 = System.Convert.ToByte("0x" + textBox_Time1.Text, 16);
                config.Filter = (Byte)(comboBox_Filter.SelectedIndex + 1);
                config.Mode = (Byte)comboBox_Mode.SelectedIndex;
                controlcan.VCI_InitCAN(m_devtype, m_devind, m_canind, ref config);
            }
            buttonConnect.Text = m_bOpen == 1 ? "断开" : "连接";
            timer_rec.Enabled = m_bOpen == 1 ? true : false;
        }

        unsafe private void timer_rec_Tick(object sender, EventArgs e)
        {
            UInt32 res = new UInt32();

            res = controlcan.VCI_Receive(m_devtype, m_devind, m_canind, ref m_recobj[0], 1000, 100);


            if (res == 0xFFFFFFFF) res = 0;//当设备未初始化时，返回0xFFFFFFFF，不进行列表显示。
            
            
            for (UInt32 i = 0; i < res; i++)
            {
                //VCI_CAN_OBJ obj = (VCI_CAN_OBJ)Marshal.PtrToStructure((IntPtr)((UInt32)pt + i * Marshal.SizeOf(typeof(VCI_CAN_OBJ))), typeof(VCI_CAN_OBJ));
                ListViewItem lvi = new ListViewItem();

                String str = "";
                lvi.ImageIndex = 1;
                lvi.Text = "0x" + m_recobj[i].ID.ToString("X2");
                if (m_recobj[i].RemoteFlag == 0)
                    lvi.SubItems.Add("数据帧");
                else
                    lvi.SubItems.Add("远程帧");
                if (m_recobj[i].ExternFlag == 0)
                    lvi.SubItems.Add("标准帧");
                else
                    lvi.SubItems.Add("扩展帧");
                //////////////////////////////////////////
                if (m_recobj[i].RemoteFlag == 0)
                {
                    
                    byte len = (byte)(m_recobj[i].DataLen % 9);
                    byte j = 0;
                    fixed (VCI_CAN_OBJ* m_recobj1 = &m_recobj[i])
                    {
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
                        if(m_recobj1->ID == 0x0A)
                        {
                            if(m_recobj1->Data[0] == 1)
                            {
                                button_brake_state1.BackColor = Color.Red;
                                button_brake_state1.Text = "制动";
                            }
                            else
                            {
                                button_brake_state1.BackColor = Color.Green;
                                button_brake_state1.Text = "解除";
                            }
                            if(m_recobj1->Data[1] == 1)
                            {
                                button_brake_state2.BackColor = Color.Red;
                                button_brake_state2.Text = "制动";
                            }
                            else
                            {
                                button_brake_state2.BackColor = Color.Green;
                                button_brake_state2.Text = "解除";
                            }
                        }
                        if(m_recobj1->ID == 0x09)
                        { 
                            textBox_motor1.Text = (m_recobj1->Data[0] | (m_recobj1 -> Data[1] << 8)).ToString();
                            textBox_motor2.Text = (m_recobj1->Data[2] | (m_recobj1->Data[3] << 8)).ToString();
                            textBox_Remote2.Text = (m_recobj1->Data[4] | (m_recobj1->Data[5] << 8)).ToString();
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
            if ((m_bOpen == 0 || cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_IDLE) || m_start == 0 )
                return;
            
            VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
            
            sendobj.RemoteFlag = 0;
            sendobj.ExternFlag = 0;
            sendobj.ID = 0x08;
            sendobj.DataLen = 8;
            UInt16 speed = (UInt16)(cHASSIS_INFO.Max_Power / 100 * trackBar_Power.Value);
            UInt16 angle = (UInt16)(cHASSIS_INFO.Max_Power / 100 * trackBar_direction.Value);
            ListViewItem lvi2 = new ListViewItem();
            lvi2.Text = "0x08";
            lvi2.ImageIndex = 0;
            lvi2.SubItems.Add( "标准帧");
            lvi2.SubItems.Add("数据帧");
            

            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RETREAT)
                speed = (UInt16)(0 - speed);
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RIGHT)
                angle = (UInt16)(0 - angle);
            if (cHASSIS_INFO.stop_flag == 1)
            {
                speed = 0;
                cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_IDLE;
                cHASSIS_INFO.stop_flag = 0;
            }
            sendobj.Data[0] = 0;
            sendobj.Data[1] = 0;
            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RETREAT || cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RUNNING)
            {
                sendobj.Data[0] = System.Convert.ToByte(speed &  0xff);
                sendobj.Data[1] = System.Convert.ToByte(speed >> 8);
            }
            sendobj.Data[2] = 0;

            sendobj.Data[3] = 0;

            if (cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_LEFT || cHASSIS_INFO.status == CHASSIS_STATUS.CHASSIS_STATUS_RIGHT)
            {
                sendobj.Data[2] = (byte)(angle & 0x00ff);
                sendobj.Data[3] = (byte)(angle >> 8);
            }

            sendobj.Data[4] = 0;

            sendobj.Data[5] = 0;

            sendobj.Data[6] = 0;

            sendobj.Data[7] = 0;
            String strdata = sendobj.Data[0].ToString("X2") +" "+ sendobj.Data[1].ToString("X2")
                + " 00 00 00 00 00 00";
            lvi2.SubItems.Add(strdata);
            listView_Info.BeginUpdate();
            listView_Info.Items.Add(lvi2);
            listView_Info.EndUpdate();
            listView_Info.EnsureVisible(listView_Info.Items.Count - 1);
            if (controlcan.VCI_Transmit(m_devtype, m_devind, m_canind, ref sendobj, 1) == 0)
            {
                MessageBox.Show("发送失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            //Marshal.FreeHGlobal(ptArray[0]);
            //Marshal.FreeHGlobal(pt);
        }

        private void button_StartCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            controlcan.VCI_StartCAN(m_devtype, m_devind, m_canind);
            m_start = 1;
        }

        private void button_StopCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            controlcan.VCI_ResetCAN(m_devtype, m_devind, m_canind);
            m_start = 0;
        }

        private void button_Clear_Click(object sender, EventArgs e)
        {
            listView_Info.Items.Clear();
        }

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

        private void button_advance_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RUNNING;
            button_advance.BackColor = System.Drawing.Color.SpringGreen;
            button_Retreat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_right.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_left.BackColor = System.Drawing.SystemColors.ButtonHighlight;
        }

        private void button_Retreat_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RETREAT;
            button_Retreat.BackColor = System.Drawing.Color.SpringGreen;
            button_advance.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_right.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_left.BackColor = System.Drawing.SystemColors.ButtonHighlight;
        }

        private void button_Stop_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            cHASSIS_INFO.stop_flag = 1;
            //cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_IDLE;
            button_turn_right.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_left.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_advance.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_Retreat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
        }

        private void button_turn_left_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            //cHASSIS_INFO.stop_flag = 1;
            //while (button_brake_state1.BackColor != Color.Red && button_brake_state2.BackColor != Color.Red) ;
            //while (cHASSIS_INFO.status != CHASSIS_STATUS.CHASSIS_STATUS_IDLE) ;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_LEFT;
            button_turn_left.BackColor = System.Drawing.Color.SpringGreen;
            button_advance.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_Retreat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_turn_right.BackColor = System.Drawing.SystemColors.ButtonHighlight;


        }

        private void button_turn_right_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            //cHASSIS_INFO.stop_flag = 1;
            //while (button_brake_state1.BackColor != Color.Red && button_brake_state2.BackColor != Color.Red) ;
            //while (cHASSIS_INFO.status != CHASSIS_STATUS.CHASSIS_STATUS_IDLE) ;
            cHASSIS_INFO.status = CHASSIS_STATUS.CHASSIS_STATUS_RIGHT;
            button_turn_right.BackColor = System.Drawing.Color.SpringGreen;
            button_turn_left.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_advance.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            button_Retreat.BackColor = System.Drawing.SystemColors.ButtonHighlight;
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

        private void trackBar_direction_Scroll(object sender, EventArgs e)
        {
            label_direction.Text = trackBar_direction.Value.ToString() + "%";
        }

        private void trackBar_Power_Scroll(object sender, EventArgs e)
        {
            label_power.Text = trackBar_Power.Value.ToString() + "%";
        }


    }
}
