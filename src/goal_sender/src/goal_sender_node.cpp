#include"ros/ros.h"
#include"move_base_msgs/MoveBaseGoal.h"
#include"geometry_msgs/Pose.h"

#include<vector>

geometry_msgs::Pose getFramePose(const std::string&, const std::string&);
double calc_distance(const geometry_msgs::Pose&, const geometry_msgs::Pose&);

struct Waypoint {
   static vector<Waypoint> readCsv(const std::string&);
   Waypoints(MoveBaseGoal, double);

   move_base_msgs::MoveBaseGoal goal;
   double valid_range;
};

class GoalSender {
   GoalSender();
   void run();
private:
   bool checkToNext();
   void sendGoalPoint();

   ros::NodeHandle n;
   ros::NodeHandle pn;
   tf::TranseformListener tf_listener;
   vector<Waypoint> waypoints;
   vector<Waypoint>::iterator now_waypoint;
};

int main(int argc, char* argv[]){
   ros::init(argc, argv, "goal_sender_node");
   GoalSender goal_sender;
   ros::Rate(10);
   while (ros::ok()) {
      goal_sender.run();
      ros::spinOnce();
   }
   return 0;
}

geometry_msgs::Pose getFramePose(tf::TransformListener& tf, const std::string& parent, const std::string& child) {
   tf::StampedTransform transform;
   try {
      tf.lookupTranseform(parent, child, ros::Time(0), transform);
   } catch (const tf::TranseformException& e) {
      ROS_ERROR("%s",ex.what());
   }
   geometry_msgs::Pose pose;
   pose.position.x = transform.getOrigin().x();
   pose.position.y = transform.getOrigin().y();
   return pose;
}

double calc_distance(const geometry_msgs::Pose a&, const geometry_msgs::Pose b&) {
   return sqrt(pow((a.position.x - b.position.x), 2.0) +
               pow((a.position.y - b.position.y), 2.0));
}

vector<Waypoint> Waypoint::readCsv(const std::string& path) {
   std::vector<Waypoint> waypoints;
   // TODO: read csv
   // TODO: if error, throw std::runtime_error
   return waypoints;
}

inline Waypoint::Waypoint(move_base_msgs::MoveBaseGoal goal, double valid_range)
:  goal(goal),
   valid_range(valid_range)
{}

GoalSender::GoalSender()
:  n(),
   pn("~"),
   tf_listener(),
   waypoints()
{
   std::string path;
   pn.getParam("path", path);
   if (path.empty()) {
      ROSINFO("I need path of waypoint");
      return -1;
   }
   try {
      waypoints = Waypoint::readCsv(path);
   } catch (const std::runtime_error& e) {
      ROSINFO("cannot read the file [%s]", e.what());
      return -1;
   }
}

void GoalSender::run() {
   if (checkToNext()) sendGoalPoint();
}

bool GoalSender::checkToNext() {
   auto robot_pos = getFramePose(tf_listener, "/map", "/base_link");
   auto waypoint_pos = now_waypoint->goal.target_pose.pose;
   auto distance = calcDistance(robot_pos, waypoint_pos);
   if (distance < now_waypoint->valid_range) { // into range
      ++now_waypoint; // next waypoint
      return true;
   }
   return false;
}

void GoalSender::sendGoalPoint() {
   now_waypoint->goal.target_pose.stamp = ros::Time::now(); // others writed by Waypoint class
   // TODO: write sendGoal method
}
