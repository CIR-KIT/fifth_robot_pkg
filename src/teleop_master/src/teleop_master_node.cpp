#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

ros::Publisher pub;

void broadcaster( const geometry_msgs::Twist::ConstPtr& msg ) {
   pub.publish(msg);
}

int main(int argc, char** argv) {
   ros::init(argc, argv, "teleop_master_node");
   ros::NodeHandle n;

   pub = n.advertise<geometry_msgs::Twist>("dest", 1);

   ros::Subscriber sub = n.subscribe("source", 100000, broadcaster);

   ros::Rate loop(100);

   while (ros::ok()){
      ros::spinOnce();
      
      loop.sleep();
   }
   return 0;
}
