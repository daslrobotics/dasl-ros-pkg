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

#include <sr_visual_servoing/visual_servoing_action_server.hpp>

namespace sr_taco
{
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
    ros::Rate rate(5.0);
    ROS_ERROR("START");
    while( ros::ok() )
    {
      feedback_ = visual_servo_->get_closer();
      servo_server_->publishFeedback(feedback_);

      rate.sleep();

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
