
#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "math.h"

double last_velocityLinks = 0.0;
double last_velocityRecht = 0.0;
double theta_last = 0.0;
double xt_last = 0.0;
double zt_last = 0.0;
double d_theta = 0.0;
ros::Time last_t;
double xt;
double zt;
double theta;
double dt;


//Funktion mit der übergebenen Variable msg vom Typ geometry_msgs::Twist
void odometryCallback(const sensor_msgs::JointState& msg)
{
	ros::Time current_t = ros::Time::now();
  ROS_INFO("Time now : [%f], last t : [%f]", current_t, last_t);
    //double current_t= current_time.toSec();
	//Speichern eines Zeitstempels in Nanosekunden
    //t = ros::Time::now().sec+ros::Time::now().nsec*1e-9;

	//Ausgabe einiger Werte in das Terminal
    ROS_INFO("vl:[%f], vr:[%f], Px:[%f], Pz:[%f]", msg.velocity[0], msg.velocity[1], msg.position[0], msg.position[1]);

    dt = (current_t - last_t).toSec();

	// Hier passiert Magie!
    // Annahme: Die Masse (m) ist 1
    //double m = 1.0; // Annahme: Einheitsmasse
    double current_velocityLinks = msg.velocity[0]; 
    double current_velocityRecht = msg.velocity[1]; 

    //double w = msg.angular.z; // Winkelgeschwindigkeit
    //double r;
    //double dv = current_velocity - last_velocity;
    //double zentrifugalkraft;
    //double translatorischkraft = m*(dv/dt);
    //if (w != 0) {
       // r = current_velocity / w; // Berechnung des Radius
        //zentrifugalkraft = m * current_velocity * current_velocity / r; // Berechnung des zentrifugalKraftes
    //}
    double b = 0.180 ; // in m distance zwischen die zwei raeder
    double r = 0.033 ; // der rad radius
    theta = ((current_velocityLinks-current_velocityRecht)*r/b)*dt + theta_last;
    d_theta = theta - theta_last;
    xt = (((current_velocityLinks+current_velocityRecht)/2)*sin(theta_last + 0.5*dt*d_theta))*dt + xt_last;
    zt = (((current_velocityLinks+current_velocityRecht)/2)*cos(theta_last + 0.5*dt*d_theta))*dt + zt_last;
    ROS_INFO("theta now: [%f], xt: [%f], zt: [%f]", theta, xt, zt);
    //ROS_INFO("Velocity: [%f], translation force: [%f]", current_velocity, translatorischkraft);

    //last_velocity = current_velocity;
    //last_time = current_time;
    theta_last = theta;
    xt_last = xt;
    zt_last = zt;

}

//Begin der Hauptmethode
int main(int argc, char **argv)
{

	//Bekanntmachen des Nodes im ROS-Master unter dem Namen fliehkraft
	//Initialisierung des ROS-Systems im Node ; Nodename und argc/argv werden mit übergeben.
  ros::init(argc, argv, "odometry");

	//Hauptverbindungspunkt zum Restlichen ROS-System (Zerstören des NodeHandle schließt den Node)
  ros::NodeHandle n;

  //Abonnieren eines topics ; hier "cmd_vel" ; Führt zum Datenaustausch mit ROS-master über publisher und subscriber
	//Nachrichten des topics werden an die Callback Funktion weitergeleitet ; hier "fliehkraftCallback"
	//Bufferspeicher für ankommende Nachrichten besitzt die Größe "1000"
	ros::Subscriber sub = n.subscribe("joint_states", 1000, odometryCallback);

	//Geht in einen loop und führt Callbacks aus. Raussprung durch Ctrl-C im Terminal oder durch den master
  ros::spin();

  return 0;
}
