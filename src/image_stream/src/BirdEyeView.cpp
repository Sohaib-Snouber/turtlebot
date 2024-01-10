#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <XmlRpcValue.h>

cv::Mat transformationMatrix; // Global variable to store the transformation matrix

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    // Convert ROS image message to OpenCV image
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat image = cv_ptr->image;

    // Apply perspective transformation using the loaded matrix
    if (!transformationMatrix.empty()) {
        cv::Mat transformedImg;
        cv::warpPerspective(image, transformedImg, transformationMatrix, image.size());

        // // Apply Gaussian blur with a specific kernel size
        // cv::Mat blurredImage;
        // cv::GaussianBlur(transformedImg, blurredImage, cv::Size(3, 3), 0); // Kernel size: 5x5

        // Convert the transformed image to binary (black and white)
        cv::Mat binaryImage;
        cv::threshold(transformedImg, binaryImage, 175, 255, cv::THRESH_BINARY);

        // Display the transformed image (if needed)
        // cv::imshow("Transformed Image", blurredImage);
        cv::imshow("Transformed Image", binaryImage);
        cv::waitKey(1);

        // Perform further processing or publish the transformed image
        // ...
    } else {
        ROS_INFO("Transformation matrix not available.");
    }
}



int main(int argc, char** argv) {
    ros::init(argc, argv, "Bird_View_node");
    ros::NodeHandle nh;

    // Load the transformation matrix from the YAML file in the config folder
    // cv::FileStorage fs("home/ubuntu/catkin_ws/src/image_stream/config/transformation_matrix.yaml", cv::FileStorage::READ);
    XmlRpc::XmlRpcValue matrix_param;
    if (nh.getParam("/transformation_matrix", matrix_param)) {
        if (matrix_param.getType() == XmlRpc::XmlRpcValue::TypeArray && matrix_param.size() == 9) {
            // Extract the matrix elements
            double transformationMatrixData[3][3];
            int k = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    transformationMatrixData[i][j] = static_cast<double>(matrix_param[k]);
                    ++k;
                }
            }

            // Copy the matrix data to a cv::Mat
            transformationMatrix = cv::Mat(3, 3, CV_64F, transformationMatrixData).clone();
        } else {
            ROS_ERROR("Transformation matrix should contain 9 elements.");
        }
    } else {
        ROS_ERROR("Failed to retrieve transformation matrix from the parameter server.");
    }

    // cv::FileStorage fs;
    // fs.open("/transformation_matrix", cv::FileStorage::READ);
    // if (fs.isOpened()) {
    //     fs["transformation_matrix"] >> transformationMatrix;
    //     fs.release();

    //     if (transformationMatrix.empty()) {
    //         ROS_INFO("Failed to load the transformation matrix.");
    //     }
    // } else {
    //     ROS_INFO("Could not open transformation matrix file.");
    // }

    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("raspicam_node/image_rect", 1, imageCallback);

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
    return 0;
}




