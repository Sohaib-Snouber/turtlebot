#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/Image.h>
#include <opencv2/calib3d/calib3d.hpp>

// Define your points here
// cv::Point2f tl(319, 510); //top left
// cv::Point2f bl(190, 663); //botom left
// cv::Point2f tr(598, 507); //top right
// cv::Point2f br(734, 654); //botom right
// cv::Point2f mil(263, 577); //middle left
// cv::Point2f mir(657, 571); // middle right
cv::Point2f tl(319.526, 510.226); //top left
cv::Point2f bl(190.670, 663.162); //botom left
cv::Point2f tr(598.175, 507.262); //top right
cv::Point2f br(733.937, 654.331); //botom right
// cv::Point2f mil(263, 577); //middle left
// cv::Point2f mir(657, 571); // middle right
// cv::Point2f tlnew(190, 663-760);
// cv::Point2f blnew(190, 663);
// cv::Point2f trnew(734, 654-760);
// cv::Point2f brnew(734, 654);
// cv::Point2f mlnew(190, 663-326);
// cv::Point2f mrnew(734, 654-326);
cv::Point2f tlnew(355.730, 55.730);
cv::Point2f blnew(355.730, 445.838);
cv::Point2f trnew(634.379, 55.730);
cv::Point2f brnew(634.379, 445.838);
// cv::Point2f mlnew(190, 663-326);
// cv::Point2f mrnew(734, 654-326);

void ImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        // Draw circles on the image based on defined points
        int radius = 10; // Example radius
        cv::Scalar color(0, 0, 255); // Red color: (B, G, R)
        cv::circle(cv_ptr->image, tl, radius, color, 2);
        cv::circle(cv_ptr->image, tr, radius, color, 2);
        cv::circle(cv_ptr->image, bl, radius, color, 2);
        cv::circle(cv_ptr->image, br, radius, color, 2);
        // cv::circle(cv_ptr->image, mil, radius, color, 2);
        // cv::circle(cv_ptr->image, mir, radius, color, 2);

        // Define coordinates of corners in the original image (front view)
        // std::vector<cv::Point2f> pts1 = {tl, tr, bl, br, mil, mir};
        std::vector<cv::Point2f> pts1 = {tl, tr, bl, br};


        // Define corresponding points for the top-down view
        // std::vector<cv::Point2f> pts2 = {tlnew, trnew, blnew, brnew, mlnew, mrnew};
        std::vector<cv::Point2f> pts2 = {tlnew, trnew, blnew, brnew};

        // Calculate Homography
        cv::Mat H = cv::findHomography(pts1, pts2);

        // Apply perspective transformation
        cv::Mat transformed_img;
        cv::warpPerspective(cv_ptr->image, transformed_img, H, cv_ptr->image.size());

        cv::imshow("Image", transformed_img);
        cv::waitKey(1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

// ... (other parts of your code)
void imageRectCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        
        // Draw circles on the image based on defined points
        int radius = 10; // Example radius
        cv::Scalar color(0, 0, 255); // Red color: (B, G, R)
        cv::circle(cv_ptr->image, tl, radius, color, 2);
        cv::circle(cv_ptr->image, tr, radius, color, 2);
        cv::circle(cv_ptr->image, bl, radius, color, 2);
        cv::circle(cv_ptr->image, br, radius, color, 2);
        // cv::circle(cv_ptr->image, mil, radius, color, 2);
        // cv::circle(cv_ptr->image, mir, radius, color, 2);


        cv::imshow("Image Rect", cv_ptr->image);
        cv::waitKey(1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub1 = nh.subscribe("raspicam_node/image_rect", 1, ImageCallback);
    ros::Subscriber sub2 = nh.subscribe("raspicam_node/image_rect", 1, imageRectCallback);

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
}
