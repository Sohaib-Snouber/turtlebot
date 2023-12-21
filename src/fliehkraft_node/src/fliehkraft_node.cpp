/*
 * cpp Datei des Projektes "fliehkraft_node"
 * author: Jan-Luca Regenhardt, Niklas Noack
 * lab: automonous mobile robotics lab (AMRL)
 * Dieses Beispiel zeigt die Abonnierung von topics, deren Verarbeitung und die Ausgabe von Ergebnissen.
*/

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "math.h"

double last_velocity = 0.0;
ros::Time last_time;

//Funktion mit der übergebenen Variable msg vom Typ geometry_msgs::Twist
void fliehkraftCallback(const geometry_msgs::Twist& msg)
{
	ros::Time current_time = ros::Time::now();
    double dt;
    double current_Time = current_time.toSec();
	//Speichern eines Zeitstempels in Nanosekunden
    // t = ros::Time::now().sec+ros::Time::now().nsec*1e-9;

	//Ausgabe einiger Werte in das Terminal
    ROS_INFO("v_x:[%f], w_z:[%f], t:[%f]", msg.linear.x, msg.angular.z, current_Time);

    dt = (current_time - last_time).toSec();

	// Hier passiert Magie!
    // Annahme: Die Masse (m) ist 1
    double m = 1.0; // Annahme: Einheitsmasse
    double current_velocity = msg.linear.x; // Lineare Geschwindigkeit
    double w = msg.angular.z; // Winkelgeschwindigkeit
    double r;
    double dv = current_velocity - last_velocity;
    double zentrifugalkraft;
    double translatorischkraft = m*(dv/dt);
    if (w != 0) {
        r = current_velocity / w; // Berechnung des Radius
        zentrifugalkraft = m * current_velocity * current_velocity / r; // Berechnung des zentrifugalKraftes
    }
    ROS_INFO("Radius: [%f], Fliehkraft: [%f]", r, zentrifugalkraft);
    ROS_INFO("Velocity: [%f], translation force: [%f]", current_velocity, translatorischkraft);

    last_velocity = current_velocity;
    last_time = current_time;
}

//Begin der Hauptmethode
int main(int argc, char **argv)
{

	//Bekanntmachen des Nodes im ROS-Master unter dem Namen fliehkraft
	//Initialisierung des ROS-Systems im Node ; Nodename und argc/argv werden mit übergeben.
  ros::init(argc, argv, "fliehkraft");

	//Hauptverbindungspunkt zum Restlichen ROS-System (Zerstören des NodeHandle schließt den Node)
  ros::NodeHandle n;

  //Abonnieren eines topics ; hier "cmd_vel" ; Führt zum Datenaustausch mit ROS-master über publisher und subscriber
	//Nachrichten des topics werden an die Callback Funktion weitergeleitet ; hier "fliehkraftCallback"
	//Bufferspeicher für ankommende Nachrichten besitzt die Größe "1000"
	ros::Subscriber sub = n.subscribe("cmd_vel", 1000, fliehkraftCallback);

	//Geht in einen loop und führt Callbacks aus. Raussprung durch Ctrl-C im Terminal oder durch den master
  ros::spin();

  return 0;
}
