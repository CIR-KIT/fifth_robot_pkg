#include <stdexcept>
#include <string>

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>
#include <goal_sender_msgs/ApplyGoals.h>
#include <goal_sender_msgs/GoalSequence.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_listener.h>

inline double squaring_distance(const geometry_msgs::Point& a, const geometry_msgs::Point& b) {
  const auto x {a.x - b.x};
  const auto y {a.y - b.y};
  return x*x + y*y;
}

/**
 * Waypoint update service manager
 */
class WaypointManager
{
public:
  WaypointManager()
    : sequence_ {},
      now_goal_ {sequence_.waypoints.end()}
  {
  }

  bool operator()(goal_sender_msgs::ApplyGoals::Request& req,
                  goal_sender_msgs::ApplyGoals::Response& res){
    sequence_ = req.goal_sequence;
    now_goal_ = sequence_.waypoints.begin();
    res.success = true;
    res.message = "update waypoints";
    return true;
  }

  const geometry_msgs::Point& point() const
  {
    if (is_end()) throw std::logic_error {"range error: Please check is_end() before point()"};
    return now_goal_->position;
  }

  const geometry_msgs::Quaternion& quaternion() const
  {
    if (is_end()) throw std::logic_error {"range error: Please check is_end() before quaternion()"};
    return now_goal_->orientation;
  }

  double radius() const
  {
    if (is_end()) throw std::logic_error {"range error: Please check is_end() before radius()"};
    return now_goal_->radius;
  }

  bool next()
  {
    if (is_end()) throw std::logic_error {"range error: Please check is_end() before next()"}; // wrong way.
    if (++now_goal_ == sequence_.waypoints.end())
      return false; // correct way: this is last one.
    return true;
  }

  bool is_end() const noexcept
  {
    return sequence_.waypoints.end() == now_goal_;
  }

  explicit operator bool() noexcept
  {
    return !is_end();
  }

  [[deprecated]]
  const goal_sender_msgs::Waypoint& get() const
  {
    if (is_end()) throw std::logic_error {"range error: Please check is_end() before get()"};
    return *now_goal_;
  }

private:
  goal_sender_msgs::GoalSequence sequence_;
  decltype(sequence_.waypoints)::iterator now_goal_;
};

/**
 * Tf lookup API
 */
class TfPositionManager
{
public:
  explicit TfPositionManager(tf2_ros::Buffer& tfBuffer) // buffer is lvalue
    : buffer_ {tfBuffer}
  {
  }

  geometry_msgs::Point operator()(std::string parent, std::string child) const
  {
    const auto ts {buffer_.lookupTransform(parent, child, ros::Time(0))};
    geometry_msgs::Point point;
    point.x = ts.transform.translation.x;
    point.y = ts.transform.translation.y;
    return point;
  }

private:
  const tf2_ros::Buffer& buffer_;
};

class GoalSender
{
public:
  using MoveBaseActionClient = actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;

  GoalSender(WaypointManager& point_manager,
             MoveBaseActionClient& move_base_client,
             TfPositionManager lookupper)
    : point_manager_ {point_manager},
      move_base_client_ {move_base_client},
      lookupper_ {lookupper}
  {
  }

  void once()
  {
    if (!point_manager_)
      return; // no work
    if (is_reach()) {
      point_manager_.next();
      send_goal();
    }
  }

private:
  bool is_reach() const
  {
    const auto robot_point {lookupper_("/map", "/base_link")};
    const auto waypoint_point {point_manager_.point()};
    const auto sqr_distance {squaring_distance(robot_point, waypoint_point)};

    const auto radius {point_manager_.radius()};
    const auto sqr_radius {radius * radius};

    if (sqr_distance < sqr_radius) // into valid range
      return true;
    return false;
  }

  void send_goal() const
  {
    if (!point_manager_) { // finish waypoint
      move_base_client_.cancelGoal(); // cancel moveing
      ROS_INFO("Finish waypoints");
      return;
    }

    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.pose.position = point_manager_.point();
    goal.target_pose.pose.orientation = point_manager_.quaternion();
    goal.target_pose.header.frame_id = "/map";
    goal.target_pose.header.stamp = ros::Time::now();
    move_base_client_.sendGoal(goal); // send waypoint
  }

  WaypointManager& point_manager_;
  MoveBaseActionClient& move_base_client_;
  TfPositionManager lookupper_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "goal_sender");
  ros::NodeHandle nh {};

  WaypointManager point_manager {};
  ros::ServiceServer srv {
      nh.advertiseService<
          goal_sender_msgs::ApplyGoals::Request,
          goal_sender_msgs::ApplyGoals::Response>(
              "apply_goals", point_manager)};

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener {tfBuffer};
  TfPositionManager lookupper {tfBuffer};

  GoalSender::MoveBaseActionClient move_base_client {"move_base", true};
  move_base_client.waitForServer();

  GoalSender goal_sender {point_manager, move_base_client, lookupper};

  ros::Rate rate {10};
  while (ros::ok()) {
    ros::spinOnce();
    goal_sender.once();
    rate.sleep();
  }
}
