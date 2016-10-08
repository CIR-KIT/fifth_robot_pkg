#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

ros::Publisher pub;

double trim_limit(double input) {
   static const double limit = 1.1;
   if (input < -limit) return -limit;
   if (limit < input) return limit;
}

void joyCb(const sensor_msgs::Joy::ConstPtr& joy) {
  geometry_msgs::Twist msg;

  msg.linear.x = trim_limit(joy->axes[0]);
  msg.linear.y = 0;
  msg.linear.z = 0;
  msg.angular.x = 0;
  msg.angular.y = 0;
  msg.angular.z = trim_limit(joy->axes[1]);
  pub.publish(msg);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "adapt_joy_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 1, joyCb);
  pub = nh.advertise<geometry_msgs::Twist>("light_fourleds", 1);
  ros::spin();
  return 0;
}
