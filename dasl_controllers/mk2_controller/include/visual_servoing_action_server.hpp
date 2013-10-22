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

#ifndef _VISUAL_SERVOING_ACTION_SERVER_HPP_
#define _VISUAL_SERVOING_ACTION_SERVER_HPP_

#include <ros/ros.h>

#include <sr_visual_servoing/visual_servoing_moveit.hpp>

#include <sr_visual_servoing/VisualServoingAction.h>
#include <sr_visual_servoing/VisualServoingFeedback.h>
#include <actionlib/server/simple_action_server.h>

namespace sr_taco
{
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
