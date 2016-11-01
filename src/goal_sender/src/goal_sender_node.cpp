#include"ros/ros.h"
#include"geometry_msgs/Pose.h"
#include"tf/transform_listener.h"
#include"move_base_msgs/MoveBaseGoal.h"
#include"move_base_msgs/MoveBaseAction.h"
#include"actionlib/client/simple_action_client.h"

#include<vector>
#include<stdexcept>

struct Waypoint;
using Waypoints = std::vector<Waypoint>;
using MoveBaseActionClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;

geometry_msgs::Pose getFramePose(const std::string&, const std::string&);
double calcDistance(const geometry_msgs::Pose&, const geometry_msgs::Pose&);

struct Waypoint {
  static std::vector<Waypoint> readCsv(const std::string&);
  Waypoint(move_base_msgs::MoveBaseGoal, double);

  move_base_msgs::MoveBaseGoal goal;
  double valid_range;
};

class GoalSender {
public:
  GoalSender();
  void run();
private:
  bool checkToNext();
  void sendGoalPoint();

  ros::NodeHandle n;
  ros::NodeHandle pn;
  tf::TransformListener tf_listener;
  Waypoints waypoints;
  Waypoints::iterator now_waypoint;
  MoveBaseActionClient move_base_client;
};

int main(int argc, char* argv[]){
  ros::init(argc, argv, "goal_sender_node");
  GoalSender goal_sender;
  ros::Rate rate {10};
  while (ros::ok()) {
    goal_sender.run();
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}

geometry_msgs::Pose getFramePose(tf::TransformListener& tf, const std::string& parent, const std::string& child) {
  tf::StampedTransform transform;
  try {
    tf.lookupTransform(parent, child, ros::Time(0), transform);
  } catch (const tf::TransformException& e) {
    ROS_ERROR("%s", e.what());
    throw std::runtime_error {"tf cannot resolve"};
  }
  geometry_msgs::Pose pose;
  pose.position.x = transform.getOrigin().x();
  pose.position.y = transform.getOrigin().y();
  return pose;
}

double calcDistance(const geometry_msgs::Pose& a, const geometry_msgs::Pose& b) {
  return sqrt(pow((a.position.x - b.position.x), 2.0) +
              pow((a.position.y - b.position.y), 2.0));
}

std::vector<Waypoint> Waypoint::readCsv(const std::string& path) {
  if (path.empty()) {
    ROS_ERROR("I need path of waypoint");
    throw std::invalid_argument {"exsist file"};
  }
  std::vector<Waypoint> waypoints;
  // TODO: read csv
  // TODO: if error, throw std::runtime_error
  return waypoints;
}

inline Waypoint::Waypoint(move_base_msgs::MoveBaseGoal goal, double valid_range)
  : goal {goal},
    valid_range {valid_range}
{}

GoalSender::GoalSender()
  : n {},
    pn {"~"},
    tf_listener {},
    waypoints {},
    now_waypoint {waypoints.begin()},
    move_base_client {"move_base", true}
{
  std::string path;
  pn.getParam("path", path);
  waypoints = Waypoint::readCsv(path); // throw std::invalid_argument, std::runtime_error
  sendGoalPoint(); // set first waypoint
  move_base_client.waitForServer();
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
  now_waypoint->goal.target_pose.header.stamp = ros::Time::now(); // others writed by Waypoint class
  // TODO: write sendGoal method
}
