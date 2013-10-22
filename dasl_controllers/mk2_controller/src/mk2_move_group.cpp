#include <moveit/move_group_interface/move_group.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit_msgs/DisplayRobotState.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

// Visualization marker
#include <visualization_msgs/Marker.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "move_group_interface_demo", ros::init_options::AnonymousName);

  // start a ROS spinning thread
  ros::AsyncSpinner spinner(1);
  spinner.start();

  //visualization_msgs::Marker msg = ros::topic::waitForMessage<visualization_msgs::Marker("target");

  // this connecs to a running instance of the move_group node
  move_group_interface::MoveGroup group("manipulator");

  // specify that our target will be a random one
  //group.setRandomTarget();
  // plan the motion and then move the group to the sampled target 

  /* POSE 1 - just below target */
  group.setEndEffectorLink("link_7");

  //print ("Marker: %.2f", msg.pose.position.x);
  geometry_msgs::PoseStamped target;
  //geometry_msgs::PoseStamped orientation;
  target=group.getCurrentPose("link_7");

  target=group.getCurrentPose("link_7");

  ROS_INFO ("Position: %f %f %f", target.pose.position.x, target.pose.position.y, target.pose.position.z);
  ROS_INFO ("Oreitation: %f %f %f %f", target.pose.orientation.x, target.pose.orientation.y, target.pose.orientation.z, target.pose.orientation.w);

// peg
//Position: 0.743636 -0.705599 0.875935
//[ INFO] [1371084037.902594813]: Oreitation: 0.002842 0.050485 -0.006812 0.998698


// 0.493841 -0.239720 -0.891920

//-0.108846 -0.251392 -0.8901130113

//0.000006 -0.000767 0.008437 0.999964

  //group.setPositionTarget(0.493841, -0.239720, -0.891920, "link_7");

//int x;

//  for (x=1; x< 10; x++)
//{

  //group.setPositionTarget(target.pose.position.x,target.pose.position.y,target.pose.position.z-0.2,"link_7");

  group.setPositionTarget (1.262997, -0.735834, 1.082077, "link_7");

  //group.setOrientationTarget(target.pose.orientation.x,target.pose.orientation.y,target.pose.orientation.z,target.pose.orientation.w,"link_7");

//Position: 1.157269 -0.754943 1.074188
//[ INFO] [1371089848.423164964]: Oreitation: -0.008948 0.699914 -0.018302 0.713937


  group.setOrientationTarget(0.023642, 0.730377, 0.011604, 0.682537, "link_7");

  //group.setRandomTarget();
  //group
  group.move();

  ros::Duration(3).sleep(); // sleep for 3 a second
//}

// x: 1.07812095206
  //      y: -1.03221465572
    //    z: 0.977435767655



//x: 0.786845757733
  //      y: -1.01246105509
    //    z: 0.803495688008



  /* POSE 2 - insert */
  //group.setEndEffectorLink("link_7");
  //group.setPositionTarget (1.08,-0.99,0.95,"link_7");
  //group.setOrientationTarget(0,0,0,1,"link_7");
  //group.move();

  //ros::Duration(7.0).sleep(); // sleep for 3 a second

/*
position: 
       x: 1.08035782901
        y: -0.993115449522
        z: 0.875297934697
      orientation: 
        x: 0.00985335585553
        z: 0.0038168834218
        y: -0.0119692125126
        w: 0.999872532242
*/


/*
position: 
        x: 0.855073749325
        y: -1.0882062302
        z: 0.968704183106
      orientation: 
        x: 0.00533579973323
        y: -0.0143309087948
        z: -0.0028900685292
        w: 0.999878893566

*/
  //std::cout<<"pose:1"<<"\n";
  /* POSE 2 */
  //group.setEndEffectorLink("link_7");
  //group.setPositionTarget (0.422572491923,-0.539405107052, -0.367110520844,"link_7");
  //group.setOrientationTarget(-0.723307655378,-0.0414544745131,-0.272436940872,0.633155332808,"link_7");
  //group.move();
  //std::cout<<"pose:2"<<"\n";
  //ros::Duration(3.0).sleep(); // sleep for 3 a second

  /* POSE 3  */
  //group.setEndEffectorLink("link_7");
  //group.setPositionTarget ( 0.463106212309,-0.558902500208,-0.45652851363,"link_7");
  //group.setOrientationTarget(-0.72330765575, -0.0414544758144,-0.272436939441,0.633155332914,"link_7");
  //group.move();

  //std::cout<<"pose:3"<<"\n";
  /* POSE 4  */
  //ros::Duration(1.5).sleep(); // sleep for 3 a second
  //group.setEndEffectorLink("link_7");
  //group.setPositionTarget (0.422572491923,-0.539405107052, -0.367110520844,"link_7");
  //group.setOrientationTarget(-0.723307655378,-0.0414544745131,-0.272436940872,0.633155332808,"link_7");
  //group.move();

  //std::cout<<"pose:4"<<"\n";
  //ros::Duration(1.5).sleep(); // sleep for 3 a second

  ros::waitForShutdown();
}



 /*

 position:  [ from stow to raised  ] 
 
        x: 0.420811293613
        y: -0.425301579226
        z: -0.390929936745
      orientation: 
        x: -0.764408581295
        y: -0.165206713917
        z: -0.131642785183
        w: 0.609144022075

+++++++++++++++++++++++++++++

 position: [ from raised to above button  ] 
        x: 0.422572491923
        y: -0.539405107052
        z: -0.367110520844
      orientation: 
        x: -0.723307655378
        y: -0.0414544745131
        z: -0.272436940872
        w: 0.633155332808

++++++++++++++++++++++++

 position: [from above the button to hitting button  ]
        x: 0.463106212309
        y: -0.558902500208
        z: -0.45652851363
      orientation: 
        x: -0.72330765575
        y: -0.0414544758144
        z: -0.272436939441
        w: 0.633155332914

+++++++++++++++++++++++++++++++

 position: [ from hitting button to above button  ] 
        x: 0.422572491923
        y: -0.539405107052
        z: -0.367110520844
      orientation: 
        x: -0.723307655378
        y: -0.0414544745131
        z: -0.272436940872
        w: 0.633155332808

-0.00663644075394
  y: -0.714747428894
  z: -0.495371937752

group.setPositionTarget (0.149414792657
,-0.631699919701,-0.667177319527
,"link_7");

*/

//leftIndexDistal
