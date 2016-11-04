#include"ros/ros.h"
#include"geometry_msgs/Pose.h"
#include"tf/transform_listener.h"
#include"move_base_msgs/MoveBaseGoal.h"
#include"move_base_msgs/MoveBaseAction.h"
#include"actionlib/client/simple_action_client.h"

#include<utility>
#include<vector>
#include<stdexcept>
#include<string>
#include<fstream> // for csv
#include<sstream> // ditto

struct Waypoint;
using WaypointContainer = std::vector<Waypoint>;
using MoveBaseActionClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;

geometry_msgs::Pose getFramePose(const std::string&, const std::string&);
double calcDistance(const geometry_msgs::Pose&, const geometry_msgs::Pose&);

struct Waypoint {
  static WaypointContainer readCsv(const std::string&);
  Waypoint(const move_base_msgs::MoveBaseGoal&, double);

  move_base_msgs::MoveBaseGoal goal;
  double valid_range;
};

class GoalSender {
public:
  GoalSender(const std::string&);
  void once();
private:
  bool checkToNext();
  void sendGoalPoint();

  WaypointContainer waypoints;
  WaypointContainer::iterator now_waypoint;
  tf::TransformListener tf_listener;
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

inline double calcDistance(const geometry_msgs::Pose& a, const geometry_msgs::Pose& b) {
  return sqrt(pow((a.position.x - b.position.x), 2.0) + pow((a.position.y - b.position.y), 2.0));
}

WaypointContainer Waypoint::readCsv(const std::string& path) {
  if (path.empty()) {
    ROS_ERROR("I need path of waypoint");
    throw std::invalid_argument {"no exist file"};
  }
  std::ifstream fs {path}; // input file stream
  if (!fs) throw std::runtime_error {"Cannot open file"};
  ROS_INFO("Reading waypoints by [%s]...", path.c_str());
  WaypointContainer waypoints;
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

inline Waypoint::Waypoint(const move_base_msgs::MoveBaseGoal& goal, double valid_range)
  : goal {goal},
    valid_range {valid_range}
{}

GoalSender::GoalSender(const std::string& path)
  : waypoints {Waypoint::readCsv(path)},
    now_waypoint {waypoints.begin()},
    tf_listener {},
    move_base_client {"move_base", true}
{
  move_base_client.waitForServer();
  sendGoalPoint(); // set first waypoint
}

inline void GoalSender::once() {
  if (checkToNext()) sendGoalPoint(); // send only when cange waypoint
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
  if (now_waypoint  == waypoints.end()) { // finish waypoint
    move_base_client.cancelGoal(); // cancel moveing
    ROS_INFO("Finish waypoints");
    return;
  }
  now_waypoint->goal.target_pose.header.stamp = ros::Time::now(); // others writed by Waypoint class
  move_base_client.sendGoal(now_waypoint->goal); // send waypoint
  ROS_INFO("Use waypoint [%ld]", now_waypoint - waypoints.begin());
}
