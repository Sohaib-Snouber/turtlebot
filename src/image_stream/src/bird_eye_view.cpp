#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <opencv2/calib3d/calib3d.hpp>

// Define your calibration parameters
cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                         851.145803, 0, 461.363994,
                         0, 853.429128, 362.519977,
                         0, 0, 1);

cv::Mat distortion_coeffs = (cv::Mat_<double>(1, 5) <<
                             0.092257, -0.273373, 0.000333, -0.000435, 0);

cv::Mat rectification_matrix = (cv::Mat_<double>(3, 3) <<
                                1, 0, 0,
                                0, 1, 0,
                                0, 0, 1);

cv::Mat projection_matrix = (cv::Mat_<double>(3, 4) <<
                              851.424927, 0, 459.501626, 0,
                              0, 858.864563, 362.178221, 0,
                              0, 0, 1, 0);

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        // Undistort the image using calibration parameters
        cv::Mat undistorted_image;
        cv::undistort(cv_ptr->image, undistorted_image, camera_matrix, distortion_coeffs);

        // Perform perspective transformation for bird's-eye view
        cv::Mat warped_image;
        cv::warpPerspective(undistorted_image, warped_image, projection_matrix, cv::Size(undistorted_image.cols, undistorted_image.rows));

        cv::imshow("Bird's Eye View", warped_image);
        cv::waitKey(1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "bird_eye_view_node");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("raspicam_node/image_rect", 1, imageCallback); 

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
    return 0;
}
