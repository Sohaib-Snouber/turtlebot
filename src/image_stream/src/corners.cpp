#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/calib3d.hpp>


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        cv::Mat gray;
        cv::cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY);

        // Define the termination criteria
        cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);

        // Prepare object points, like (0,0,0), (1,0,0), (2,0,0), ..., (6,5,0)
        std::vector<cv::Point3f> objp;
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 9; ++j) {
                objp.push_back(cv::Point3f(j, i, 0));
            }
        }

        std::vector<cv::Point2f> corners;
        bool patternFound = cv::findChessboardCorners(gray, cv::Size(6, 9), corners);

        if (patternFound) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            for (size_t i = 0; i < corners.size(); ++i) {
                cv::putText(cv_ptr->image, std::to_string(i), corners[i], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
                //cv::putText(cv_ptr->image, "(" + std::to_string(corners[i].x) + ", " + std::to_string(corners[i].y) + ")", corners[i] + cv::Point2f(10, -10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
            }

            cv::drawChessboardCorners(cv_ptr->image, cv::Size(6, 9), corners, patternFound);

            // Print the corner points and their positions
            for (size_t i = 0; i < corners.size(); ++i) {
                std::cout << "Corner " << i << ": (" << corners[i].x << ", " << corners[i].y << ")" << std::endl;
            }


            // // Draw corners on the image
            // cv::drawChessboardCorners(cv_ptr->image, cv::Size(9, 14), corners, patternFound);

            // // Print the corner points
            // for (const auto& corner : corners) {
            //     std::cout << "Corner: (" << corner.x << ", " << corner.y << ")" << std::endl;
            // }

            cv::imshow("Image_rect", cv_ptr->image);
            cv::waitKey(1);
        }
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_rect_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("raspicam_node/image_rect", 1, imageCallback);

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
}
