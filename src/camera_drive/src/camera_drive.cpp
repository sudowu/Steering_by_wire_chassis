#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/header.hpp>
// Lyrical 中 cv_bridge 头文件已更名为 cv_bridge.hpp
// #include <cv_bridge/cv_bridge.h>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

class ImagePublisher : public rclcpp::Node
{
public:
    ImagePublisher() : Node("image_publisher"), cap_(0)
    {
        pub_ = create_publisher<sensor_msgs::msg::Image>("image_raw", 10);
        timer_ = create_wall_timer(std::chrono::milliseconds(10), // 100Hz
                                   std::bind(&ImagePublisher::publish, this));
        if (!cap_.isOpened())
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to open camera.");
            rclcpp::shutdown();
            return;
        }
    }

private:
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    cv::VideoCapture cap_;

    void publish()
    {
        cv::Mat frame;
        if (cap_.read(frame) && !frame.empty())
        {
            // 从摄像头读取成功，直接使用读取的帧
            auto msg = cv_bridge::CvImage(
                           std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
            msg->header.stamp = this->now();
            msg->header.frame_id = "camera"; // 随便起个坐标系名
            pub_->publish(*msg);
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Failed to read from camera, publishing random image.");
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImagePublisher>());
    rclcpp::shutdown();
    return 0;
}
