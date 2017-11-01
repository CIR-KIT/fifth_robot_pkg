#include "ros/ros.h"
#include "actionlib/client/simple_action_client.h"
#include "geometry_msgs/Pose.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "move_base_msgs/MoveBaseGoal.h"
#include "tf/transform_listener.h"

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>

geometry_msgs::Pose getFramePose(const std::string&, const std::string&);
double calcDistance(const geometry_msgs::Pose&, const geometry_msgs::Pose&);

struct Waypoint {
  using Container = std::vector<Waypoint>;
  static Container readCsv(std::string&&);
  Waypoint(move_base_msgs::MoveBaseGoal&&, double);

  move_base_msgs::MoveBaseGoal goal;
  double valid_range;
};

class GoalSender {
public:
  GoalSender(std::string&&);
  void once();
  bool isFinishWaypoint();
private:
  bool checkToNext();
  void sendGoalPoint();
  Waypoint::Container::size_type nowPointNumber();

  Waypoint::Container waypoints;
  Waypoint::Container::iterator now_waypoint;
  tf::TransformListener tf_listener;
  using MoveBaseActionClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;
  MoveBaseActionClient move_base_client;
};

int main(int argc, char* argv[]){
  ros::init(argc, argv, "goal_sender_node");
  ros::NodeHandle pn {"~"};
  std::string path;
  pn.getParam("path", path);
  GoalSender goal_sender {std::move(path)};
  ros::Rate rate {10};
  while (ros::ok()) {
    ros::spinOnce();
    goal_sender.once();
    if (goal_sender.isFinishWaypoint()) break; // go to end program
    rate.sleep();
  }
  return 0;
}

geometry_msgs::Pose getFramePose(const tf::TransformListener& tf, const std::string& parent, const std::string& child) {
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

inline double calcDistance(const geometry_msgs::Pose& a, const geometry_msgs::Pose& b) {
  return sqrt(pow((a.position.x - b.position.x), 2.0) + pow((a.position.y - b.position.y), 2.0));
}

Waypoint::Container Waypoint::readCsv(std::string&& path) { // but not move now
  if (path.empty()) {
    ROS_ERROR("I need path of waypoint");
    throw std::invalid_argument {"no exist file"};
  }
  std::ifstream fs {path}; // input file stream
  if (!fs) throw std::runtime_error {"Cannot open file"};
  ROS_INFO("Reading waypoints by [%s]...", path.c_str());
  Container waypoints;
  for (std::string line; std::getline(fs, line);) {
    if (line.empty()) break; // skip the empty line
    std::istringstream line_stream {line}; // convert to stream
    std::vector<double> input_data;
    auto&& input_it = back_inserter(input_data);
    for (std::string oneData; std::getline(line_stream, oneData, ',');) {
      std::istringstream data_st {oneData}; // convert to stream
      double data;
      data_st >> data;
      *input_it = data;
      ++input_it;
    }
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.pose.position.x    = input_data[0];
    goal.target_pose.pose.position.y    = input_data[1];
    goal.target_pose.pose.position.z    = input_data[2];
    goal.target_pose.pose.orientation.x = input_data[3];
    goal.target_pose.pose.orientation.y = input_data[4];
    goal.target_pose.pose.orientation.z = input_data[5];
    goal.target_pose.pose.orientation.w = input_data[6];
    goal.target_pose.header.frame_id = "/map";
    waypoints.emplace_back(std::move(goal), input_data[7]);
  }
  ROS_INFO("Finish read Waypoint [%lu]", waypoints.size());
  return waypoints;
}

inline Waypoint::Waypoint(move_base_msgs::MoveBaseGoal&& goal, double valid_range)
  : goal {std::move(goal)},
    valid_range {valid_range}
{}

GoalSender::GoalSender(std::string&& path)
  : waypoints {Waypoint::readCsv(std::move(path))},
    now_waypoint {waypoints.begin()},
    tf_listener {},
    move_base_client {"move_base", true}
{
  const auto& now_pos = now_waypoint->goal.target_pose.pose;
  ROS_INFO("Set first waypoint: x:[%f], y:[%f]", now_pos.position.x, now_pos.position.y);
  move_base_client.waitForServer();
  sendGoalPoint(); // set first waypoint
}

inline void GoalSender::once() {
  if (checkToNext()) {
    sendGoalPoint(); // send only when cange waypoint
    const auto& now_pos = now_waypoint->goal.target_pose.pose;
    ROS_INFO("Update waypoint number [%lu]: x:[%f], y:[%f]", nowPointNumber(), now_pos.position.x, now_pos.position.y);
  }
}

inline bool GoalSender::isFinishWaypoint() {
  return now_waypoint == waypoints.end();
}

bool GoalSender::checkToNext() {
  const auto& robot_pos = getFramePose(tf_listener, "/map", "/base_link");
  const auto& waypoint_pos = now_waypoint->goal.target_pose.pose;
  const auto& distance = calcDistance(robot_pos, waypoint_pos); // distance of between robot and target
  if (distance < now_waypoint->valid_range) { // into range
    ++now_waypoint; // next waypoint
    return true;
  }
  return false;
}

void GoalSender::sendGoalPoint() {
  if (isFinishWaypoint()) { // finish waypoint
    move_base_client.cancelGoal(); // cancel moveing
    ROS_INFO("Finish waypoints");
    return;
  }
  now_waypoint->goal.target_pose.header.stamp = ros::Time::now(); // others writed by Waypoint class
  move_base_client.sendGoal(now_waypoint->goal); // send waypoint
  ROS_INFO("Use waypoint [%ld]", now_waypoint - waypoints.begin());
}

Waypoint::Container::size_type GoalSender::nowPointNumber() {
  return now_waypoint - waypoints.begin();
}
