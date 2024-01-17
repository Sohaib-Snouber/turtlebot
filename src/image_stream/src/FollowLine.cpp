#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/Twist.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

ros::Publisher cmdVelPublisher;


int height = 720;
int width = 960;
int squareWidth = width / 12;
int squareHeight = height / 20;
int center_box = width / 2 - squareWidth / 2;
int left_box = center_box - squareWidth;
int right_box = center_box + squareWidth;

// Variables to store the average intensity of each box
double averageLeftBox = 0.0;
double averageCenterBox = 0.0;
double averageRightBox = 0.0;
double pos_difference;
double all_average;
double umgebung_licht;

void binaryImageCallback(const sensor_msgs::ImageConstPtr& msg) {
    // Convert ROS image message to OpenCV image
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    // Extract regions of interest (ROIs) for each box
    cv::Mat leftBoxROI = cv_ptr->image(cv::Rect(left_box, height - (squareHeight+5), squareWidth, squareHeight));
    cv::Mat centerBoxROI = cv_ptr->image(cv::Rect(center_box, height - (squareHeight+5), squareWidth, squareHeight));
    cv::Mat rightBoxROI = cv_ptr->image(cv::Rect(right_box, height - (squareHeight+5), squareWidth, squareHeight));
    
    // Draw rectangles around the ROIs on the output image
    cv::rectangle(cv_ptr->image, cv::Rect(left_box, height - (squareHeight + 5), squareWidth, squareHeight), cv::Scalar(0), 1);
    cv::rectangle(cv_ptr->image, cv::Rect(center_box, height - (squareHeight + 5), squareWidth, squareHeight), cv::Scalar(0), 1);
    cv::rectangle(cv_ptr->image, cv::Rect(right_box, height - (squareHeight + 5), squareWidth, squareHeight), cv::Scalar(0), 1);

    // Calculate the average intensity of each box
    averageLeftBox = cv::mean(leftBoxROI)[0];
    averageCenterBox = cv::mean(centerBoxROI)[0];
    averageRightBox = cv::mean(rightBoxROI)[0];
    all_average = (averageLeftBox + averageCenterBox + averageRightBox)/3;
    umgebung_licht = all_average/255;
    // Print the averages
    ROS_INFO("left average Value: %f", averageLeftBox);
    ROS_INFO("center average Value: %f", averageCenterBox);
    ROS_INFO("right average Value: %f", averageRightBox);

   
    // 0.22rad/sec ist die max geschwindigkeit fuer beide linear und angular speeds.
    // +0.22 angular ist links , und -0.22 ist rechts
    geometry_msgs::Twist twistMsg;
    // twistMsg.linear.x = 0.12*110/averageCenterBox;
    // if (averageLeftBox > averageRightBox){
    //     pos_difference = averageRightBox - averageLeftBox;
    //     twistMsg.angular.z = 0.12*pos_difference/50;
    // } else {
    //     pos_difference = averageRightBox - averageLeftBox;
    //     twistMsg.angular.z = 0.12*pos_difference/50;
    // }
    twistMsg.linear.x = ((255-averageCenterBox) / 1700) * umgebung_licht;
    twistMsg.angular.z =(averageRightBox - averageLeftBox)/150;
    
    if (averageLeftBox > 200 && averageCenterBox > 200 && averageRightBox > 200){
        twistMsg.linear.x = -0.025;
    }
    

    // // Publish velocity based on pixel color
    // if (averageCenterBox <= 200) {  // Assuming black pixel has a value of 0
    //     // Set velocity for black color
    //     twistMsg.linear.x = 5/averageCenterBox;  // Adjust the linear velocity as needed
    // } else {
    //     // Set velocity for white color
    //     twistMsg.linear.x = 0.0;  // Stop the robot for white color
    // }
    // if (averageLeftBox <= 200) {  // Assuming black pixel has a value of 0
    //     // Set velocity for black color
    //     twistMsg.angular.z = 5/averageLeftBox;  // Adjust the linear velocity as needed
    // } 
    // if (averageRightBox <= 200) {  // Assuming black pixel has a value of 0
    //     // Set velocity for black color
    //     twistMsg.angular.z= -5/averageRightBox;  // Adjust the linear velocity as needed

    // Display the image
    cv::imshow("Binary Image", cv_ptr->image);
    cv::waitKey(1);

    // Publish the velocity
    cmdVelPublisher.publish(twistMsg);
    

    
    // Display the image
    cv::imshow("Binary Image", cv_ptr->image);
    cv::waitKey(1);

    // Publish the velocity
    cmdVelPublisher.publish(twistMsg);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "color_check_node");
    ros::NodeHandle nh;

    // Subscribe to the binary image topic
    ros::Subscriber binaryImageSubscriber = nh.subscribe("binary_image", 1, binaryImageCallback);

    // Advertise the cmd_vel topic
    cmdVelPublisher = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);

    ros::spin();

    return 0;
}
