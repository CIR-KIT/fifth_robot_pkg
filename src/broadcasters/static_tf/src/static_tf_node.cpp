#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "static_tf_node");
  ros::NodeHandle n;
  ros::NodeHandle np("~");

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  double quat_x, quat_y, quat_z, quat_w;
  np.param("quat/x", quat_x, 0.0);
  np.param("quat/y", quat_y, 0.0);
  np.param("quat/z", quat_z, 0.0);
  np.param("quat/w", quat_w, 1.0);

  double v_x,v_y,v_z;
  np.param("vector/x", v_x, 0.1);
  np.param("vector/y", v_y, 0.0);
  np.param("vector/z", v_z, 0.1);

  while(n.ok()){
    broadcaster.sendTransform(
        tf::StampedTransform(
          tf::Transform(tf::Quaternion(quat_x,quat_y,quat_z,quat_w), tf::Vector3(v_x,v_y,v_z)),
          ros::Time::now(),
          "base_link",
          "base_laser"
        )
    );
    r.sleep();
  }
}
