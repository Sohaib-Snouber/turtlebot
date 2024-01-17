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

        // Define square parameters
        int height = 720;
        int width = 960;
        int squareWidth = width/8;
        int squareHight = height/10;
        int center_box = width/2 - squareWidth/2;
        int left_box = center_box - squareWidth;
        int right_box = center_box + squareWidth;
        
        cv::circle(cv_ptr->image, cv::Point(width, height), 5, cv::Scalar(255), -1);

        // Draw red squares at specific coordinates
        cv::rectangle(cv_ptr->image, cv::Point(left_box , height - squareHight), cv::Point(center_box, height), cv::Scalar(0, 0, 255), -1);
        cv::rectangle(cv_ptr->image, cv::Point(center_box, height - squareHight), cv::Point(right_box, height), cv::Scalar(0, 255, 0), -1);
        cv::rectangle(cv_ptr->image, cv::Point(right_box, height - squareHight), cv::Point(right_box + squareWidth, height), cv::Scalar(255, 0, 0), -1);

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



