#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/CompressedImage.h>



void imageCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        cv::imshow("Image", cv_ptr->image);
        cv::waitKey(1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->format.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub1 = nh.subscribe("raspicam_node/image/compressed", 1, imageCallback);

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
}