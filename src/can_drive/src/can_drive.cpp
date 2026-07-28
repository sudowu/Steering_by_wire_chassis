#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "controlcan.h"

class CanDriveNode : public rclcpp::Node
{
public:
    CanDriveNode() : Node("can_drive_node")
    {

        publisher_ = this->create_publisher<std_msgs::msg::String>("can_drive_topic", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&CanDriveNode::publish_message, this));
    }

private:

    void publish_message()
    {
        auto message = std_msgs::msg::String();
        message.data = "Hello from CanDriveNode!";
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published message: '%s'", message.data.c_str());
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    VCI_BOARD_INFO pInfo;//用来获取设备信息。
    int count=0;//数据列表中，用来存储列表序号。
    VCI_BOARD_INFO pInfo1 [50];

};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CanDriveNode>());
    rclcpp::shutdown();
return 0;
}