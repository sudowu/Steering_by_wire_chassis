#include "rclcpp/rclcpp.hpp"
#include "can_msgs/msg/frame.hpp"
#include "controlcan.h"

class CanDriveNode : public rclcpp::Node
{
public:
    CanDriveNode() : Node("can_drive_node")
    {
        Findusbdevice();
        OpenDevice();
        InitCan();
        StartCan();
        publisher_ = this->create_publisher<can_msgs::msg::Frame>("can_drive_receive", 10);
        subscription_ = this->create_subscription<can_msgs::msg::Frame>(
            "can_drive_send",
            10,
            std::bind(&CanDriveNode::handle_message, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&CanDriveNode::publish_message, this));
    }

private:
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr publisher_;
    rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;

    VCI_BOARD_INFO pInfo; // 用来获取设备信息。
    int count = 0;        // 数据列表中，用来存储列表序号。
    VCI_BOARD_INFO pInfo1[50];
    
    VCI_INIT_CONFIG pInitConfig;         // 用来初始化CAN的数据类型。
    VCI_FILTER_RECORD pFilterRecord[50]; // 用来存储过滤器信息。

    void publish_message()
    {
        VCI_CAN_OBJ pCanObj[2500];           // 用来存储接收的CAN信息帧。
        int len = VCI_Receive(VCI_USBCAN2A, 0, 0, pCanObj, 2500, 0);
        
        if (len > 0)
        {
            for (int i = 0; i < len; i++)
            {
                RCLCPP_INFO(this->get_logger(),
                            "Received CAN message: ID=0x%X, DataLen=%d, Data=[%02X %02X %02X %02X %02X %02X %02X %02X]",
                            pCanObj[i].ID,
                            pCanObj[i].DataLen,
                            pCanObj[i].Data[0],
                            pCanObj[i].Data[1],
                            pCanObj[i].Data[2],
                            pCanObj[i].Data[3],
                            pCanObj[i].Data[4],
                            pCanObj[i].Data[5],
                            pCanObj[i].Data[6],
                            pCanObj[i].Data[7]);
                auto message = can_msgs::msg::Frame();
                message.id = pCanObj[i].ID;
                message.is_rtr = pCanObj[i].RemoteFlag;
                message.is_extended = pCanObj[i].ExternFlag;
                message.is_error = false; // Assuming no error for now
                message.dlc = pCanObj[i].DataLen;
                for (int j = 0; j < pCanObj[i].DataLen; j++)
                {
                    message.data[j] = pCanObj[i].Data[j];
                }
                message.header.stamp = this->now();
                message.header.frame_id = "can_frame";
                publisher_->publish(message);
            }
        }
        else if (len == 0)
        {
            RCLCPP_WARN(this->get_logger(), "No CAN messages received.");
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Error receiving CAN messages.");
            VCI_CloseDevice(VCI_USBCAN2A, 0);
            rclcpp::shutdown();
        }
    }

    void handle_message(const can_msgs::msg::Frame ::SharedPtr msg)
    {
        if(rclcpp::ok() == false)
        {
            RCLCPP_ERROR(this->get_logger(), "ROS is not ok, shutting down.");
            VCI_CloseDevice(VCI_USBCAN2A, 0);
            rclcpp::shutdown();
            return;
        }
        // 在这里处理接收到的消息，例如发送到CAN总线
        VCI_CAN_OBJ sendObj;
        sendObj.ID = msg->id;
        sendObj.RemoteFlag = msg->is_rtr;
        sendObj.ExternFlag = msg->is_extended;
        sendObj.DataLen = msg->dlc;
        for (int i = 0; i < msg->dlc; i++)
        {
            sendObj.Data[i] = msg->data[i];
        }
        int result = VCI_Transmit(VCI_USBCAN2A, 0, 0, &sendObj, 1);
        if (result == 1)
        {        
            RCLCPP_INFO(this->get_logger(),
                    "Received message to send: ID=0x%X, DataLen=%d, Data=[%02X %02X %02X %02X %02X %02X %02X %02X]",
                    msg->id,
                    msg->dlc,
                    msg->data[0],
                    msg->data[1],
                    msg->data[2],
                    msg->data[3],
                    msg->data[4],
                    msg->data[5],
                    msg->data[6],
                    msg->data[7]);
        }
        else if(result == -1)
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to send message.");
            VCI_CloseDevice(VCI_USBCAN2A, 0);
            rclcpp::shutdown();
        }
    }
    /**
     * Finds the USB device.
     * @return 0 if successful, -1 otherwise.
     */
    void Findusbdevice()
    {
        int num = VCI_FindUsbDevice2(&pInfo);
        if (num > 0)
        {
            RCLCPP_INFO(this->get_logger(), "Found %d CAN devices.", num);
            for (int i = 0; i < num; ++i)
            {
                RCLCPP_INFO(this->get_logger(), "Device %d: Serial Number: %s, Hardware Type: %s",
                            i + 1, pInfo.str_Serial_Num, pInfo.str_hw_Type);
            }
            return;
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "No CAN devices found.");
            RCLCPP_ERROR(this->get_logger(), "Failed to find USB device.");
            rclcpp::shutdown();
            return;
        }
    }

    void OpenDevice()
    {
        int result = VCI_OpenDevice(VCI_USBCAN2A, 0, 0);
        if (result == 1)
        {
            RCLCPP_INFO(this->get_logger(), "Device opened successfully.");
            return;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to open device.");
            rclcpp::shutdown();
            return;
        }
    }

    void InitCan()
    {
        pInitConfig.AccCode = 0x00000000;
        pInitConfig.AccMask = 0xFFFFFFFF;
        pInitConfig.Filter = 0;     // 接收所有帧
        pInitConfig.Timing0 = 0x00; // 波特率设置
        pInitConfig.Timing1 = 0x14; // 波特率设置
        pInitConfig.Mode = 0;       // 正常模式

        int result = VCI_InitCAN(VCI_USBCAN2A, 0, 0, &pInitConfig);
        if (result == 1)
        {
            RCLCPP_INFO(this->get_logger(), "CAN initialized successfully.");
            return;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize CAN.");
            rclcpp::shutdown();
            return;
        }
    }
    void StartCan()
    {
        int result = VCI_StartCAN(VCI_USBCAN2A, 0, 0);
        if (result == 1)
        {
            RCLCPP_INFO(this->get_logger(), "CAN started successfully.");
            return;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to start CAN.");
            rclcpp::shutdown();
            return;
        }
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CanDriveNode>());
    rclcpp::shutdown();
    return 0;
}