#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <goal_sender_msgs/ApplyGoals.h>
#include <tf2_ros/transform_listener.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "goal_sender");

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener {tfBuffer};
  geometry_msgs::TransformStamped transformStamped;
  try{
    transformStamped = tfBuffer.lookupTransform("turtle2", "turtle1", ros::Time(0));
  } catch (tf2::TransformException &ex) {
    ROS_WARN("%s",ex.what());
    ros::Duration(1.0).sleep();
  }
}
