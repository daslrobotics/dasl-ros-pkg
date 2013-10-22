/**
 * @file   visual_servoing.cpp
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

#include <sr_visual_servoing/visual_servoing.hpp>
#include <sr_visual_servoing/utils.hpp>
#include <ros/package.h>

#include <std_msgs/Float64.h>

//#include <omp.h>

namespace sr_taco
{
  //TODO get rid of this const, use a tf lookup or something
  const double VisualServoing::ARM_HEIGHT_CONST_ = 1.02;

  VisualServoing::VisualServoing()
    : nh_tilde_("~"),
      arm_velocity_(-1.0),
      object_msg_received_(false),
      joint_states_msg_received_(false)
  {
    init_openrave_();

    init_robot_publishers_();

    // the distance is set to -1 until we've started tracking the object
    visual_servoing_feedback_.distance = -1.0;

    //initialises subscribers and timer
    joint_states_sub_ = nh_tilde_.subscribe("/joint_states", 2, &VisualServoing::joint_states_cb_, this);

    odom_sub_ = nh_tilde_.subscribe("/analyse_moving_object/odometry", 2, &VisualServoing::new_odom_cb_, this);
  }

  VisualServoing::~VisualServoing()
  {
    OpenRAVE::RaveDestroy();
  }

  ///Servoes the arm to the current tracked_object
  sr_visual_servoing::VisualServoingFeedback VisualServoing::get_closer()
  {
    //don't do anything if we haven't received the first tracked_object
    // or the first joint_states msg.
    if( object_msg_received_ && joint_states_msg_received_ )
    {
      //generate different solutions aroung the current position
      // and keep the one closest to object position + twist
      // (the object is moving toward this point)
      generate_best_solution_();

      //send it to the joints
      send_robot_targets_();

      //updates the feedback message
      update_feedback_();
    }

    return visual_servoing_feedback_;
  }

  void VisualServoing::new_odom_cb_(const nav_msgs::OdometryConstPtr& msg)
  {
    //update the target
    tracked_object_.pose = msg->pose;
    tracked_object_.twist = msg->twist;
    tracked_object_.child_frame_id = msg->child_frame_id;
    tracked_object_.header = msg->header;
    tracked_object_.header.frame_id = "shadowarm_base";
    object_msg_received_ = true;
  }

  void VisualServoing::update_feedback_()
  {
    //update the feedback with the tracked object pose;
    visual_servoing_feedback_.object_pose.position.x = tracked_object_.pose.pose.position.x;
    visual_servoing_feedback_.object_pose.position.y = tracked_object_.pose.pose.position.y;
    visual_servoing_feedback_.object_pose.position.z = tracked_object_.pose.pose.position.z;

    visual_servoing_feedback_.object_pose.orientation.x = tracked_object_.pose.pose.orientation.x;
    visual_servoing_feedback_.object_pose.orientation.y = tracked_object_.pose.pose.orientation.y;
    visual_servoing_feedback_.object_pose.orientation.z = tracked_object_.pose.pose.orientation.z;
    visual_servoing_feedback_.object_pose.orientation.w = tracked_object_.pose.pose.orientation.w;

    OpenRAVE::Transform trans = rave_manipulator_->GetEndEffectorTransform();
    //The local tool is where we want the grasping point to be (defined in arm_and_hand_motor.xml)
    trans.trans += rave_manipulator_->GetLocalToolTransform().trans;

    //update the feedback with the grasping point transform
    visual_servoing_feedback_.grasp_pose.position.x = trans.trans.x;
    visual_servoing_feedback_.grasp_pose.position.y = trans.trans.y;

    visual_servoing_feedback_.grasp_pose.position.z = trans.trans.z - ARM_HEIGHT_CONST_;

    visual_servoing_feedback_.grasp_pose.orientation.x = trans.rot.x;
    visual_servoing_feedback_.grasp_pose.orientation.y = trans.rot.y;
    visual_servoing_feedback_.grasp_pose.orientation.z = trans.rot.z;
    visual_servoing_feedback_.grasp_pose.orientation.w = trans.rot.w;

    visual_servoing_feedback_.cartesian_velocity = arm_velocity_;

    //update the distance between the object and the tooltip in the feedback
    visual_servoing_feedback_.distance = compute_distance_(visual_servoing_feedback_.grasp_pose.position, visual_servoing_feedback_.object_pose.position );

  }

  void VisualServoing::generate_best_solution_()
  {
    boost::mutex::scoped_lock l(mutex_);

    //TODO: use openmp for loop (https://computing.llnl.gov/tutorials/openMP/#DO)

    OpenRAVE::Transform end_effector = rave_manipulator_->GetEndEffectorTransform();
    end_effector.trans += rave_manipulator_->GetLocalToolTransform().trans;

    arm_velocity_ = compute_cartesian_velocity_(end_effector);

    last_end_effector_pose_ = end_effector;

    OpenRAVE::Transform object, twist;
    object.trans.x = tracked_object_.pose.pose.position.x;
    object.trans.y = tracked_object_.pose.pose.position.y;

    object.trans.z = tracked_object_.pose.pose.position.z + ARM_HEIGHT_CONST_;

    twist.trans.x = tracked_object_.twist.twist.linear.x;
    twist.trans.y = tracked_object_.twist.twist.linear.y;
    twist.trans.z = tracked_object_.twist.twist.linear.z;

    /*
     * We're aiming for the point at which the object will
     *  be when we get there, based on the arm velocity.
     *  The closer we are to the object, the
     *  closer we're going toward the object. The further
     *  away we are the more in front of the object we're
     *  going to aim for.
     */
    double distance = OpenRAVE::geometry::MATH_SQRT( (object.trans - end_effector.trans).lengthsqr3() );

    OpenRAVE::Transform target;
    // object + twist * time it would take to get to the object
    double reaching_time = distance / arm_velocity_;
    target.trans = object.trans + twist.trans * reaching_time;

    ROS_DEBUG_STREAM("Distance = " << distance << " (reaching in approx "<< reaching_time
                     << "s at"<< arm_velocity_ <<"m.s-1), end effector: "<< end_effector.trans <<" / object: "<< object.trans
                     <<" / twist: " << twist.trans << "=> " << target.trans);

    //Fixed orientation of the wrist
    target.rot.w = 0.5599;
    target.rot.x = 0.4320;
    target.rot.y = 0.4320;
    target.rot.z = 0.5599;

    //TODO: should get closer to the target if it can't reach it.

    //try a few random orientations around the target
    // to have more chances of finding a solution
    for( unsigned int i=0; i < 50; ++i)
    {
      //randomize orientation around the target
      // to make sure we find a solution
      target.rot.x += (OpenRAVE::RaveRandomFloat() - 0.5) / 5.0;
      target.rot.y += (OpenRAVE::RaveRandomFloat() - 0.5) / 5.0;
      target.rot.z += (OpenRAVE::RaveRandomFloat() - 0.5) / 5.0;
      target.rot.w += (OpenRAVE::RaveRandomFloat() - 0.5) / 5.0;

      target.rot.normalize();

      std::vector<OpenRAVE::dReal> ik_solution;
      try
      {
        if( rave_manipulator_->FindIKSolution(OpenRAVE::IkParameterization(target), ik_solution, OpenRAVE::IKFO_IgnoreEndEffectorCollisions) )
        {
          std::stringstream ss;
          for(size_t i = 0; i < ik_solution.size(); ++i)
          {
            robot_targets_[i] = ik_solution[i];
            ss << ik_solution[i] << " ";
          }

          ROS_DEBUG_STREAM("The solution for: " << target << " \n  is: " << ss.str());

          return;
        }
      }
      catch(OpenRAVE::openrave_exception& e)
      {
        ROS_ERROR_STREAM( "Caught Openrave exception[" << e.GetCode() << "]: " << e.message() );
      }
    }

    ROS_DEBUG_STREAM("No solution found for " << target
                     << " \n current pos: " << rave_manipulator_->GetEndEffectorTransform());
  }

  void VisualServoing::send_robot_targets_()
  {
    if( joint_states_msg_received_ )
    {
      ROS_ASSERT(target_names_.size() == robot_targets_.size());
      std_msgs::Float64 msg;
      for (unsigned int i = 0; i < target_names_.size(); ++i)
      {
        msg.data = robot_targets_[i];
        robot_publishers_[joint_names_[i]].publish( msg );
      }
    }
  }

  double VisualServoing::compute_cartesian_velocity_(OpenRAVE::Transform end_effector)
  {
    double velocity = 0.0;
    if( arm_velocity_ == -1.0 ) //last pose was not initialised return a velocity of 0
      return velocity;

    double distance = OpenRAVE::geometry::MATH_SQRT( (last_end_effector_pose_.trans - end_effector.trans).lengthsqr3() );
    ros::Time current_time = ros::Time::now();
    ros::Duration time_difference = current_time - last_time_;
    last_time_ = current_time;
    velocity = distance / (time_difference.toSec());

    return velocity;
  }


  void VisualServoing::joint_states_cb_(const sensor_msgs::JointStateConstPtr& msg)
  {
    boost::mutex::scoped_lock l(mutex_);

    if( !joint_states_msg_received_ )
    {
      joint_names_ = msg->name;

      joint_states_msg_received_ = true;

      //set the active DOFs for the robot the first time we receive a message.
      rave_manipulator_->GetRobot()->SetActiveDOFs(rave_manipulator_->GetArmIndices());
    }

    std::vector<OpenRAVE::dReal> current_position;
    ROS_ASSERT( msg->position.size() == joint_names_.size() );
    for(unsigned int i=0; i < joint_names_.size(); ++i)
    {
      current_positions_[ joint_names_[i] ] = msg->position[i];
      current_position.push_back(OpenRAVE::dReal(msg->position[i]));
    }

    try
    {
      //update the openrave model position
      rave_manipulator_->GetRobot()->SetActiveDOFValues( current_position, 0 );
    }
    catch(OpenRAVE::openrave_exception& e)
    {
      ROS_ERROR_STREAM( "Caught Openrave exception[" << e.GetCode() << "]: " << e.message() );
    }
  }

  void VisualServoing::init_openrave_()
  {
    OpenRAVE::RaveInitialize(true);
    rave_env_ = OpenRAVE::RaveCreateEnvironment();

    //lock the environment to prevent changes
    OpenRAVE::EnvironmentMutex::scoped_lock lock(rave_env_->GetMutex());
    //load the scene from the xml file
    // the file is in sr_visual_servoing/openrave/arm_and_hand_motor.xml
    std::string path = ros::package::getPath("sr_visual_servoing");
    path += "/openrave/arm_and_hand_motor.xml";
    rave_robot_ = rave_env_->ReadRobotXMLFile(path);
    if( !rave_robot_ )
    {
      rave_env_->Destroy();
      ROS_FATAL("Couldn't create the robot for OpenRAVE");
      ROS_BREAK();
    }

    rave_env_->Add(rave_robot_);
    rave_ikfast_ = OpenRAVE::RaveCreateModule(rave_env_, "ikfast");
    rave_env_->Add(rave_ikfast_, true, "");
    //rave_robot_->SetActiveManipulator("arm_and_hand_motor");

    std::stringstream ssin,ssout;
    ssin << "LoadIKFastSolver " << rave_robot_->GetName() << " " << OpenRAVE::IKP_Transform6D;
    rave_manipulator_ = rave_robot_->GetActiveManipulator();
    if( !rave_ikfast_->SendCommand(ssout, ssin) )
    {
      ROS_FATAL_STREAM( "failed to load the ik solver for " << rave_robot_->GetName() << ssout.str() );
      rave_env_->Destroy();
      ROS_BREAK();
    }
  }

  void VisualServoing::init_robot_publishers_()
  {
    //Those are the targets to which we'll publish for
    // moving the arm (we also include the wrist in the arm)
    target_names_.push_back("ShoulderJRotate");
    robot_targets_.push_back(0.0);
    target_names_.push_back("ShoulderJSwing");
    robot_targets_.push_back(0.0);
    target_names_.push_back("ElbowJSwing");
    robot_targets_.push_back(0.0);
    target_names_.push_back("ElbowJRotate");
    robot_targets_.push_back(0.0);
    target_names_.push_back("WRJ2");
    robot_targets_.push_back(0.0);
    target_names_.push_back("WRJ1");
    robot_targets_.push_back(0.0);

    //initialises the map of publishers
    //TODO: really ugly, replace by a service call etc...
    //for the hand
    robot_publishers_["FFJ0"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_ffj0_mixed_position_velocity_controller/command", 1);
    robot_publishers_["FFJ3"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_ffj3_mixed_position_velocity_controller/command", 1);
    robot_publishers_["FFJ4"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_ffj4_mixed_position_velocity_controller/command", 1);

    robot_publishers_["MFJ0"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_mfj0_mixed_position_velocity_controller/command", 1);
    robot_publishers_["MFJ3"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_mfj3_mixed_position_velocity_controller/command", 1);
    robot_publishers_["MFJ4"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_mfj4_mixed_position_velocity_controller/command", 1);

    robot_publishers_["RFJ0"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_rfj0_mixed_position_velocity_controller/command", 1);
    robot_publishers_["RFJ3"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_rfj3_mixed_position_velocity_controller/command", 1);
    robot_publishers_["RFJ4"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_rfj4_mixed_position_velocity_controller/command", 1);

    robot_publishers_["LFJ0"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_lfj0_mixed_position_velocity_controller/command", 1);
    robot_publishers_["LFJ3"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_lfj3_mixed_position_velocity_controller/command", 1);
    robot_publishers_["LFJ4"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_lfj4_mixed_position_velocity_controller/command", 1);
    robot_publishers_["LFJ5"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_lfj5_mixed_position_velocity_controller/command", 1);

    robot_publishers_["THJ1"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_thj1_mixed_position_velocity_controller/command", 1);
    robot_publishers_["THJ2"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_thj2_mixed_position_velocity_controller/command", 1);
    robot_publishers_["THJ3"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_thj3_mixed_position_velocity_controller/command", 1);
    robot_publishers_["THJ4"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_thj4_mixed_position_velocity_controller/command", 1);
    robot_publishers_["THJ5"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_thj5_mixed_position_velocity_controller/command", 1);

    robot_publishers_["WRJ1"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_wrj1_mixed_position_velocity_controller/command", 1);
    robot_publishers_["WRJ2"] = nh_tilde_.advertise<std_msgs::Float64>("/sh_wrj2_mixed_position_velocity_controller/command", 1);

    // for the arm
    robot_publishers_["ShoulderJRotate"] = nh_tilde_.advertise<std_msgs::Float64>("/sa_sr_position_controller/command", 1);
    robot_publishers_["ShoulderJSwing"] = nh_tilde_.advertise<std_msgs::Float64>("/sa_ss_position_controller/command", 1);
    robot_publishers_["ElbowJRotate"] = nh_tilde_.advertise<std_msgs::Float64>("/sa_er_position_controller/command", 1);
    robot_publishers_["ElbowJSwing"] = nh_tilde_.advertise<std_msgs::Float64>("/sa_es_position_controller/command", 1);
  }

  //////////
  // ACTION SERVER


  VisualServoingActionServer::VisualServoingActionServer()
  {
    visual_servo_.reset( new VisualServoing() );

    servo_server_.reset( new VisualServoServer(nh_, "visual_servo", boost::bind(&VisualServoingActionServer::execute, this, _1), false) );
    servo_server_->registerPreemptCallback(boost::bind(&VisualServoingActionServer::preempt, this));

    servo_server_->start();
  }

  VisualServoingActionServer::~VisualServoingActionServer()
  {
  }

  void VisualServoingActionServer::preempt()
  {
    ROS_DEBUG("GOAL PREEMPTED");

    servo_server_->setPreempted();
  }

  void VisualServoingActionServer::execute(const sr_visual_servoing::VisualServoingGoalConstPtr& goal)
  {
    while( ros::ok() )
    {
      ros::Duration(0.1).sleep();

      feedback_ = visual_servo_->get_closer();
      servo_server_->publishFeedback(feedback_);

      if( !servo_server_->isActive() )
      {
        ROS_DEBUG("ABORTING");
        return;
      }
    }

    //will never reach this point as the goal is infinite
    servo_server_->setSucceeded();
  }

}//end namespace sr_taco




int main(int argc, char *argv[])
{
  ros::init(argc, argv, "visual_servoing");

  sr_taco::VisualServoingActionServer visual_servo_as;
  ros::spin();
}

/* For the emacs weenies in the crowd.
   Local Variables:
   c-basic-offset: 2
   End:
*/
