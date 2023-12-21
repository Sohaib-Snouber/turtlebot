#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"

class RadiusCalculator {
public:
    RadiusCalculator() {
        // Initialize the subscriber and publisher
        vel_sub = nh.subscribe("velocity_topic", 10, &RadiusCalculator::velocityCallback, this);
        radius_pub = nh.advertise<std_msgs::Float64>("radius", 10);
    }

    void velocityCallback(const geometry_msgs::Twist::ConstPtr& msg) {
        double linear_velocity = msg->linear.x;
        double angular_velocity = msg->angular.z;

        // Make sure angular velocity is not zero to avoid division by zero
        if (angular_velocity != 0) {
            std_msgs::Float64 radius_msg;
            radius_msg.data = linear_velocity / angular_velocity;
            radius_pub.publish(radius_msg);
        }
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber vel_sub;
    ros::Publisher radius_pub;
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "radius_calculator_node");
    RadiusCalculator calc;

    ros::spin();

    return 0;
}
