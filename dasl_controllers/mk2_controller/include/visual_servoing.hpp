/**
 * @file   visual_servoing.hpp
 * @author Ugo Cupcic <ugo@shadowrobot.com>
 * @date   Tue Sep  4 14:57:11 2012
 *
*
* Copyright 2011 Shadow Robot Company Ltd.
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
 * @brief This node is used to servo the arm and hand on a moving object.
 * It will try to grasp the object if possible.
 *
 * The principle we'll use is the following:
 *  - the callback simply updates the target
 *  - a fast loop is trying to get closer to the target iteratively
 *
 */

#ifndef _VISUAL_SERVOING_HPP_
#define _VISUAL_SERVOING_HPP_

#include <ros/ros.h>

#include <boost/smart_ptr.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>
#include <map>

#include <openrave-core.h>

#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/JointState.h>

//#include "sr_visual_servoing/kinematics/ik_fast.h"

#include <sr_visual_servoing/VisualServoingAction.h>
#include <sr_visual_servoing/VisualServoingFeedback.h>
#include <actionlib/server/simple_action_server.h>

namespace sr_taco
{
  class VisualServoing
  {
  public:
    VisualServoing();
    ~VisualServoing();

    ///Servo the arm to the current target, called periodically from Actionlib Server
    sr_visual_servoing::VisualServoingFeedback get_closer();

  protected:
    static const double ARM_HEIGHT_CONST_;

    ros::NodeHandle nh_tilde_;

    ///subscribes to the joint_states, updating the vector of current_positions_
    ros::Subscriber joint_states_sub_;
    std::map<std::string, double> current_positions_;
    std::vector<std::string> joint_names_;

    void joint_states_cb_(const sensor_msgs::JointStateConstPtr& msg);

    ///subscribes to the odometry messages coming from the object analyser
    ros::Subscriber odom_sub_;
    void new_odom_cb_(const nav_msgs::OdometryConstPtr& msg);

    /**
     * Generate different solutions aroung the current position
     *  and keep the one closest to object position + twist
     *  (the object is moving toward this point)
     *
     *  Updates the robot_targets_ vector.
     */
    void generate_best_solution_();

    /**
     * Send the current robot_targets_ to the robot.
     */
    void send_robot_targets_();
    /**
     * map containing the publishers for sending targets to the robot
     */
    std::map<std::string, ros::Publisher> robot_publishers_;
    void init_robot_publishers_();

    /**
     * Initialises OpenRAVE tools for computing IK.
     */
    void init_openrave_();
    OpenRAVE::EnvironmentBasePtr rave_env_;
    OpenRAVE::RobotBasePtr rave_robot_;
    OpenRAVE::ModuleBasePtr rave_ikfast_;
    OpenRAVE::RobotBase::ManipulatorPtr rave_manipulator_;
    OpenRAVE::ViewerBasePtr rave_viewer_;
    OpenRAVE::IkSolverBasePtr rave_ik_solver_;

    ///The cartesian velocity of the arm
    double arm_velocity_;
    double compute_cartesian_velocity_(OpenRAVE::Transform end_effector);
    OpenRAVE::Transform last_end_effector_pose_;
    ros::Time last_time_;

    ///The latest object position and twist
    nav_msgs::Odometry tracked_object_;

    ///The targets we'll send to the robot
    std::vector<double> robot_targets_;
    ///The names of the joints we're sending targets to
    std::vector<std::string> target_names_;

    ///Set to true once we've received a msg
    bool object_msg_received_;
    bool joint_states_msg_received_;

    ///A mutex to keep joint states cb and compute method threadsafe
    boost::mutex mutex_;

    ///update the feedback for the action server
    void update_feedback_();
    sr_visual_servoing::VisualServoingFeedback visual_servoing_feedback_;

    static inline double compute_distance_(geometry_msgs::Point a, geometry_msgs::Point b)
    {
      return std::sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
    }
  };

  typedef actionlib::SimpleActionServer<sr_visual_servoing::VisualServoingAction> VisualServoServer;

  class VisualServoingActionServer
  {
  public:
    VisualServoingActionServer();
    virtual ~VisualServoingActionServer();

    void execute(const sr_visual_servoing::VisualServoingGoalConstPtr& goal);
    void preempt();
  protected:
    boost::shared_ptr<VisualServoing> visual_servo_;
    boost::shared_ptr<VisualServoServer> servo_server_;
    ros::NodeHandle nh_;

    sr_visual_servoing::VisualServoingFeedback feedback_;
  };

} //end namespace sr_taco

/* For the emacs weenies in the crowd.
   Local Variables:
   c-basic-offset: 2
   End:
*/

#endif
