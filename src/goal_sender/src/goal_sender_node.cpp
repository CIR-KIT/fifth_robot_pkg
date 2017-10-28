#include <string>

#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <goal_sender_msgs/ApplyGoals.h>
#include <goal_sender_msgs/GoalSequence.h>
#include <tf2_ros/transform_listener.h>

/**
 * Waypoint update service manager
 */
class WaypointManager
{
public:
  bool operator()(goal_sender_msgs::ApplyGoals::Request& req,
                  goal_sender_msgs::ApplyGoals::Response& res){
    return true;
  }

  goal_sender_msgs::GoalSequence get() const
  {
    return sequence_;
  }

  goal_sender_msgs::GoalSequence operator()() const
  {
    return get();
  }

private:
  goal_sender_msgs::GoalSequence sequence_;
};

/**
 * Tf lookup API
 */
class TfPositionManager
{
public:
  TfPositionManager(tf2_ros::Buffer& tfBuffer)
    : buffer_ {tfBuffer}
  {
  }

  geometry_msgs::TransformStamped operator()(std::string parent, std::string child) const
  {
    return buffer_.lookupTransform(parent, child, ros::Time(0));
  }

private:
  tf2_ros::Buffer& buffer_;
};

class GoalSender
{
public:
  GoalSender(WaypointManager point_manager,
             TfPositionManager lookupper)
    : point_manager_ {point_manager},
      lookupper_ {lookupper}
  {
  }

  void once()
  {
  }

private:
  WaypointManager point_manager_;
  TfPositionManager lookupper_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "goal_sender");
  ros::NodeHandle nh {};

  WaypointManager point_manager {};
  ros::ServiceServer srv {
      nh.advertiseService<goal_sender_msgs::ApplyGoals::Request,
                          goal_sender_msgs::ApplyGoals::Response>(
          "apply_goals", point_manager)};

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener {tfBuffer};
  TfPositionManager lookupper {tfBuffer};

  GoalSender goal_sender {point_manager, lookupper};
}
