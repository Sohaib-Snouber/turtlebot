#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/LaserScan.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
// #include <opencv2/core/persistence.hpp>
#include <XmlRpcValue.h>
#include <opencv2/calib3d/calib3d.hpp>

cv::Mat transformationMatrix; // Global variable to store the transformation matrix
std::vector<cv::Point3f> global3DPoints; // Global variable to store 3D points

image_transport::Publisher binaryImagePublisher; // Publisher for the binary image

void store3DPoints(const sensor_msgs::LaserScanConstPtr& scanMsg) {
    // Clear the global 3D points vector
    global3DPoints.clear();

    // Iterate through the laser scan data
    for (int i = 0; i < scanMsg->ranges.size(); ++i) {
        // Skip invalid measurements
        if (std::isinf(scanMsg->ranges[i]) || std::isnan(scanMsg->ranges[i])) {
            continue;
        }

        // Calculate the angle in radians
        double angle = (scanMsg->angle_min + (i * scanMsg->angle_increment));

        // Calculate the x, y, z coordinates in the world
        double x = scanMsg->ranges[i] * sin(angle);
        double y = scanMsg->ranges[i] * cos(angle);
        double z = 0.18; // the height of the lidar sensor over ground

        // rotation der Koordinaten
        double y_cam = -z;
        double z_cam = y;
        // x spiegeln
        x = -x;

        // translation der Koordinaten (Verschiebung Laserscanner zu Kamera)
        double z_img = z_cam + 0.07;
        double y_img = y_cam + 0.07;
        // ROS_INFO("%f",z_img); // Entfehrnung ausgeben

        // verschiebung an Fusspunkt von Objekt
        y_img = y_img + 0.25;

        // z > 0 damit die Punkte hinter dem Roboter von cv.projectPoints nicht auch transformiert werden
        if (z_img>0.25 && z_img<1 && x>-0.5 && x<0.5)
        {
            // Store the 3D point in the global vector
            global3DPoints.push_back(cv::Point3f(x, y_img, z_img));
        }
        else {
            // damit arraz sonst nicht leer ist
            global3DPoints.push_back(cv::Point3f(10, 10, 0));
        }
    }
}

void apply3DPointsToImage(const sensor_msgs::ImageConstPtr& msg) {
    // Convert ROS image message to OpenCV image
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat image = cv_ptr->image;
    cv::Mat transformedImg;

    // Apply perspective transformation using the loaded matrix
    if (!transformationMatrix.empty()) {
        // Check if there are 3D points available
        if (global3DPoints.empty()) {
            ROS_WARN("No 3D points available for projection.");
            return;
        }

        // Camera calibration parameters
        cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 851.145803, 0, 461.363994, 0, 853.429128, 362.519977, 0, 0, 1);
        cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << 0.092257, -0.273373, 0.000333, -0.000435, 0);

        // // Rotation parameters
        // double rotationAngle = 90.0;  // Rotate by 90 degrees
        // cv::Point3f rotationAxis(0.0, 0.0, 1.0);  // Rotate around the Z-axis

        // // Rotate each 3D point before projecting
        // std::vector<cv::Point3f> rotated3DPoints;
        // for (const auto& point : global3DPoints) {
        //     cv::Point3f rotatedPoint = rotatePoint(point, rotationAngle, rotationAxis);
        //     rotated3DPoints.push_back(rotatedPoint);
        // }
        
        // Convert 3D points to 2D image points
        std::vector<cv::Point2f> imagePoints;
        cv::projectPoints(global3DPoints, cv::Vec3d(0, 0, 0), cv::Vec3d(0, 0, 0), cameraMatrix, distCoeffs, imagePoints);
        // Draw the projected points on the image
        for (const auto& point : imagePoints) {
            cv::circle(image, point, 5, cv::Scalar(255, 255, 255), -1);
        }
        // Publish the binary image
        sensor_msgs::ImagePtr binaryImageMsg = cv_bridge::CvImage(std_msgs::Header(), "mono8", image).toImageMsg();
        binaryImagePublisher.publish(binaryImageMsg);

        cv::warpPerspective(image, transformedImg, transformationMatrix, image.size());

        // // Apply Gaussian blur with a specific kernel size
        // cv::GaussianBlur(transformedImg, blurredImage, cv::Size(3, 3), 0); // Kernel size: 5x5

        // // Convert the transformed image to binary (black and white)
        // cv::Mat binaryImage;
        // //cv::threshold(transformedImg, binaryImage, 160, 255, cv::THRESH_BINARY);
        // //Gauss-Filter gegen Rauschen
        // blur = cv.GaussianBlur(transformedImg,(5,5),0)
        // // Binaerbild eryeugen mit Otsu's Thresholderkennung
        // cv::threshold(blur, binaryImage, 0, 255, cv.THRESH_BINARY+cv.THRESH_OTSU);

        // Display the transformed image (if needed)
        // cv::imshow("Transformed Image", blurredImage);
        // cv::imshow("Transformed Image", binaryImage);
        // cv::waitKey(1);
        // Display the transformed image (if needed)
        cv::imshow("Transformed Image", transformedImg);
        cv::waitKey(1);
        cv::imshow("full view", image);
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

    // Advertise the binary image topic
    image_transport::ImageTransport it(nh);
    binaryImagePublisher = it.advertise("binary_image", 1);

    // Subscribe to the original image topic
    image_transport::Subscriber subImage = it.subscribe("raspicam_node/image_rect", 1, apply3DPointsToImage);

    // Subscribe to the scan topic to store 3D points
    ros::Subscriber subScan = nh.subscribe("scan", 1, store3DPoints);

    cv::startWindowThread();

    ros::spin();

    cv::destroyAllWindows();
    return 0;
}


// void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
//     // Convert ROS image message to OpenCV image
//     cv_bridge::CvImagePtr cv_ptr;
//     try {
//         cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
//     } catch (cv_bridge::Exception& e) {
//         ROS_ERROR("cv_bridge exception: %s", e.what());
//         return;
//     }

//     cv::Mat image = cv_ptr->image;

//     // Apply perspective transformation using the loaded matrix
//     if (!transformationMatrix.empty()) {
//         cv::Mat transformedImg;
//         cv::warpPerspective(image, transformedImg, transformationMatrix, image.size());

//         // // Apply Gaussian blur
//         // cv::Mat blur;
//         // cv::GaussianBlur(transformedImg, blur, cv::Size(5, 5), 0);

//         // // Convert the transformed image to binary (black and white)
//         // cv::Mat binaryImage;
//         // cv::threshold(transformedImg, binaryImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

//         // Publish the binary image
//         sensor_msgs::ImagePtr binaryImageMsg = cv_bridge::CvImage(std_msgs::Header(), "mono8", transformedImg).toImageMsg();
//         binaryImagePublisher.publish(binaryImageMsg);

//         // Display the transformed image (if needed)
//         cv::imshow("Transformed Image", transformedImg);
//         cv::waitKey(1);

//         // Perform further processing or publish the transformed image
//         // ...
//     } else {
//         ROS_INFO("Transformation matrix not available.");
//     }
// }

// int main(int argc, char** argv) {
//     ros::init(argc, argv, "Bird_View_node");
//     ros::NodeHandle nh;

//     // Load the transformation matrix from the YAML file in the config folder
//     XmlRpc::XmlRpcValue matrix_param;
//     if (nh.getParam("/transformation_matrix", matrix_param)) {
//         if (matrix_param.getType() == XmlRpc::XmlRpcValue::TypeArray && matrix_param.size() == 9) {
//             // Extract the matrix elements
//             double transformationMatrixData[3][3];
//             int k = 0;
//             for (int i = 0; i < 3; ++i) {
//                 for (int j = 0; j < 3; ++j) {
//                     transformationMatrixData[i][j] = static_cast<double>(matrix_param[k]);
//                     ++k;
//                 }
//             }

//             // Copy the matrix data to a cv::Mat
//             transformationMatrix = cv::Mat(3, 3, CV_64F, transformationMatrixData).clone();
//         } else {
//             ROS_ERROR("Transformation matrix should contain 9 elements.");
//         }
//     } else {
//         ROS_ERROR("Failed to retrieve transformation matrix from the parameter server.");
//     }

//     // Advertise the binary image topic
//     image_transport::ImageTransport it(nh);
//     binaryImagePublisher = it.advertise("binary_image", 1);

//     // Subscribe to the original image topic
//     image_transport::Subscriber sub = it.subscribe("raspicam_node/image_rect", 1, imageCallback);

//     cv::startWindowThread();

//     ros::spin();

//     cv::destroyAllWindows();
//     return 0;
// }





// #include <ros/ros.h>
// #include <cv_bridge/cv_bridge.h>
// #include <image_transport/image_transport.h>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/calib3d/calib3d.hpp>
// #include <opencv2/core.hpp>
// #include <opencv2/core/persistence.hpp>
// #include <XmlRpcValue.h>

// cv::Mat transformationMatrix; // Global variable to store the transformation matrix

// void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
//     // Convert ROS image message to OpenCV image
//     cv_bridge::CvImagePtr cv_ptr;
//     try {
//         cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
//     } catch (cv_bridge::Exception& e) {
//         ROS_ERROR("cv_bridge exception: %s", e.what());
//         return;
//     }

//     cv::Mat image = cv_ptr->image;

//     // Apply perspective transformation using the loaded matrix
//     if (!transformationMatrix.empty()) {
//         cv::Mat transformedImg;
//         cv::warpPerspective(image, transformedImg, transformationMatrix, image.size());

//         // // Apply Gaussian blur with a specific kernel size
//         // cv::Mat blurredImage;
//         // cv::GaussianBlur(transformedImg, blurredImage, cv::Size(3, 3), 0); // Kernel size: 5x5

//         // Convert the transformed image to binary (black and white)
//         cv::Mat binaryImage;
//         //cv::threshold(transformedImg, binaryImage, 160, 255, cv::THRESH_BINARY);
//         //Gauss-Filter gegen Rauschen
//         blur = cv.GaussianBlur(transformedImg,(5,5),0)
//         // Binaerbild eryeugen mit Otsu's Thresholderkennung
//         cv::threshold(blur, binaryImage, 0, 255, cv.THRESH_BINARY+cv.THRESH_OTSU);

//         // Display the transformed image (if needed)
//         // cv::imshow("Transformed Image", blurredImage);
//         cv::imshow("Transformed Image", binaryImage);
//         cv::waitKey(1);

//         // Perform further processing or publish the transformed image
//         // ...
//     } else {
//         ROS_INFO("Transformation matrix not available.");
//     }
// }



// int main(int argc, char** argv) {
//     ros::init(argc, argv, "Bird_View_node");
//     ros::NodeHandle nh;

//     // Load the transformation matrix from the YAML file in the config folder
//     // cv::FileStorage fs("home/ubuntu/catkin_ws/src/image_stream/config/transformation_matrix.yaml", cv::FileStorage::READ);
//     XmlRpc::XmlRpcValue matrix_param;
//     if (nh.getParam("/transformation_matrix", matrix_param)) {
//         if (matrix_param.getType() == XmlRpc::XmlRpcValue::TypeArray && matrix_param.size() == 9) {
//             // Extract the matrix elements
//             double transformationMatrixData[3][3];
//             int k = 0;
//             for (int i = 0; i < 3; ++i) {
//                 for (int j = 0; j < 3; ++j) {
//                     transformationMatrixData[i][j] = static_cast<double>(matrix_param[k]);
//                     ++k;
//                 }
//             }

//             // Copy the matrix data to a cv::Mat
//             transformationMatrix = cv::Mat(3, 3, CV_64F, transformationMatrixData).clone();
//         } else {
//             ROS_ERROR("Transformation matrix should contain 9 elements.");
//         }
//     } else {
//         ROS_ERROR("Failed to retrieve transformation matrix from the parameter server.");
//     }

    
//     image_transport::ImageTransport it(nh);
//     image_transport::Subscriber sub = it.subscribe("raspicam_node/image_rect", 1, imageCallback);

//     cv::startWindowThread();

//     ros::spin();

//     cv::destroyAllWindows();
//     return 0;
// }

// // cv::FileStorage fs;
//     // fs.open("/transformation_matrix", cv::FileStorage::READ);
//     // if (fs.isOpened()) {
//     //     fs["transformation_matrix"] >> transformationMatrix;
//     //     fs.release();

//     //     if (transformationMatrix.empty()) {
//     //         ROS_INFO("Failed to load the transformation matrix.");
//     //     }
//     // } else {
//     //     ROS_INFO("Could not open transformation matrix file.");
//     // }


