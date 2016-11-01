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
using Waypoints = std::vector<Waypoint>;
using MoveBaseActionClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;

geometry_msgs::Pose getFramePose(const std::string&, const std::string&);
double calcDistance(const geometry_msgs::Pose&, const geometry_msgs::Pose&);

struct Waypoint {
  static Waypoints readCsv(const std::string&);
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

inline double calcDistance(const geometry_msgs::Pose& a, const geometry_msgs::Pose& b) {
  return sqrt(pow((a.position.x - b.position.x), 2.0) +
              pow((a.position.y - b.position.y), 2.0));
}

Waypoints Waypoint::readCsv(const std::string& path) {
  if (path.empty()) {
    ROS_ERROR("I need path of waypoint");
    throw std::invalid_argument {"exsist file"};
  }
  std::ifstream fs {path}; // input file stream
  if (!fs) throw std::runtime_error {"Cannot open file"};
  std::string line;
  Waypoints waypoints;
  while (std::getline(fs, line)) {
    if (line.empty()) break; // skip the empty line
    std::istringstream line_stream {std::move(line)}; // convert to stream
    std::vector<double> input_data;
    auto input_it = back_inserter(input_data);
    std::string oneData;
    while (std::getline(line_stream, oneData, ',')) {
      std::istringstream data_st {std::move(oneData)}; // convert to stream
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
    waypoints.push_back(Waypoint {std::move(goal), input_data[7]});
  }
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
  try {
    waypoints = Waypoint::readCsv(path); // throw std::invalid_argument, std::runtime_error
  } catch (const std::runtime_error& e) {
    ROS_ERROR("%s [%s]", e.what(), path.c_str());
    throw;
  }
  move_base_client.waitForServer();
  sendGoalPoint(); // set first waypoint
}

inline void GoalSender::run() {
  if (checkToNext()) sendGoalPoint(); // send only when cange waypoint
}

bool GoalSender::checkToNext() {
  auto robot_pos = getFramePose(tf_listener, "/map", "/base_link");
  auto waypoint_pos = now_waypoint->goal.target_pose.pose;
  auto distance = calcDistance(robot_pos, waypoint_pos); // distance of between robot and target
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
