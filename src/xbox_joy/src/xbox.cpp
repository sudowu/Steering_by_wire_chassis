#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

class XboxJoy : public rclcpp::Node
{
public:
    XboxJoy() : Node("xbox_joy")
    {
        subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "joy", 10, std::bind(&XboxJoy::joyCallback, this, std::placeholders::_1));
    }

private:
    void joyCallback(const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(),
                    "Received Joy message: \r\n"
                    "L_X:%f, L_Y:%f, LT:%f, R_X:%f, R_Y:%f, RT:%f\r\n"
                    "A:%d, B:%d, X:%d, Y:%d\r\n"
                    "LB:%d, RB:%d, Back:%d, Start:%d\r\n"
                    "Guide:%d, L_Stick:%d, R_Stick:%d",
                    msg->axes[0], msg->axes[1], msg->axes[2], msg->axes[3], msg->axes[4], msg->axes[5],
                    msg->buttons[0], msg->buttons[1], msg->buttons[2], msg->buttons[3], msg->buttons[4],
                    msg->buttons[5], msg->buttons[6], msg->buttons[7], msg->buttons[8], msg->buttons[9],
                    msg->buttons[10]);
    }

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<XboxJoy>());
    rclcpp::shutdown();
    return 0;
}
