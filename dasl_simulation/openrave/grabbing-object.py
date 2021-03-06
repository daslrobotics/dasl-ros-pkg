#!/usr/bin/env python
""" Test Program """

import time
import openravepy
if not __openravepy_build_doc__:
    from openravepy import *
    from numpy import *
    from math import pi

def waitrobot(robot):
    """busy wait for robot completion"""
    while not robot.GetController().IsDone():
        time.sleep(0.01)

def main(env,options):
    "Main example code."
    # load a scene from ProjectRoom environment XML file
    env.Load('gantry.env.xml')
    time.sleep(1)

    #recorder = RaveCreateModule(env,'viewerrecorder')
    #env.AddModule(recorder,'')
    #filename = 'openrave.mpg'
    #codec = 13 # mpeg2
    #recorder.SendCommand('Start 640 480 30 codec %d timing realtime filename %s\nviewer %s'%(codec,filename,env.GetViewer().GetName()))

    # 1) get the 1st robot that is inside the loaded scene
    # 2) assign it to the variable named 'robot'
    robot = env.GetRobots()[0]

    # select manipulator and IK model
    manip = robot.SetActiveManipulator('leftarm') # set the manipulator to leftarm
    ikmodel = databases.inversekinematics.InverseKinematicsModel(robot,iktype=IkParameterization.Type.Transform6D)
    if not ikmodel.load():
        ikmodel.autogenerate()

    # create the interface for basic manipulation programs
    basemanip = interfaces.BaseManipulation(robot,plannername=options.planner)
    taskprob = interfaces.TaskManipulation(robot,plannername=options.planner)

    print 'move arms to intial position'

    #target=env.GetKinBody('TibitsBox1')
    target=env.GetKinBody('peg')

    with env:
        jointnames = ['l_j0','l_j1','l_j2','l_j3','l_j4','l_j5','l_j6',
                      'r_j0','r_j1','r_j2','r_j3','r_j4','r_j5','r_j6']
        robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
        basemanip.MoveActiveJoints(goal=[-pi/4, pi/2, pi/2, -pi/2, 0, -pi/2, 0,
                                          pi/4, pi/2, pi/2,  pi/2, 0,  pi/2, 0])
    waitrobot(robot)

    print 'move robot base to target'
    with env:
        robot.SetActiveDOFs([],DOFAffine.X|DOFAffine.Y|DOFAffine.RotationAxis,[0,0,1])
        basemanip.MoveActiveJoints(goal=[3.0,-1.6,0],maxiter=5000,steplength=0.15,maxtries=2)
        jointnames = ['x_axis_joint','y_axis_joint','z_axis_joint']
        robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
        basemanip.MoveActiveJoints(goal=[8.7,-1.5,0.2])
    waitrobot(robot)

    print 'release fingers'
    taskprob.ReleaseFingers()
    waitrobot(robot)

    #raw_input('press ENTER to continue...');

    #raw_input('press ENTER to continue...');

    gmodel = databases.grasping.GraspingModel(robot,target)
    if not gmodel.load():
        gmodel.autogenerate()
    
    validgrasps, validindicees = gmodel.computeValidGrasps(returnnum=1)
    basemanip = interfaces.BaseManipulation(robot)

    with robot:
        grasp = validgrasps[0]
        gmodel.setPreshape(grasp)
        T = gmodel.getGlobalGraspTransform(grasp,collisionfree=True)
        traj = basemanip.MoveToHandPosition(matrices=[T],execute=False,outputtrajobj=True)

    raveLogInfo('traj has %d waypoints, last waypoint is: %s'%(traj.GetNumWaypoints(),repr(traj.GetWaypoint(-1))))
    robot.GetController().SetPath(traj)
    robot.WaitForController(0)

    #    validgrasps, validindicees = gmodel.computeValidGrasps(returnnum=1)
    #    gmodel.moveToPreshape(validgrasps[0])
    #    Tgoal = gmodel.getGlobalGraspTransform(validgrasps[0],collisionfree=True)
    #    basemanip = interfaces.BaseManipulation(robot)
    #    basemanip.MoveToHandPosition(matrices=[Tgoal])
    #    robot.WaitForController(0)
    #    taskmanip = interfaces.TaskManipulation(robot)
    #    taskmanip.CloseFingers()
    #robot.WaitForController(0)

    raw_input('press ENTER to continue...');

    exit(1)

    #Tgoal = array([[0,-1,0,3.5],[-1,0,0,-1.3],[0,0,-1,0.842],[0,0,0,1]])

    print 'move the arm to the target'
    #with env:
        #jointnames = ['l_j0','l_j1','l_j2','l_j3','l_j4','l_j5','l_j6','x_axis_joint','y_axis_joint','z_axis_joint']
        #robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])

    Tgoal = array([[0,-1,0,3.485],
                   [-1,0,0,-1.3],
                   [0,0,-1,1.1],
                   [0,0,0,1]])

#    Tgoal = array([[0,-1,0,3.5],
#                   [-1,0,0,-0.5],
#                   [0,0,-1,1.5],
#                   [0,0,0,1]])
    res = basemanip.MoveToHandPosition(matrices=[Tgoal],seedik=10)
    waitrobot(robot)

    raw_input('press ENTER to continue...');

    print 'close fingers until collision'
    taskprob.CloseFingers()
    waitrobot(robot)

    print 'move the arm with the target back to the initial position'
    with env:
        robot.Grab(target)
        #basemanip.MoveManipulator(zeros(len(manip.GetArmIndices())),jitter=0.04)
        basemanip.MoveManipulator(goal=[-pi/4, pi/2, pi/2, -pi/2, 0, -pi/2, 0])
    waitrobot(robot)

    print 'move the robot to another location'
    with env:
        jointnames = ['x_axis_joint','y_axis_joint','z_axis_joint']
        robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
        basemanip.MoveActiveJoints(goal=[8.7,0.7,0.2])
        #robot.SetActiveDOFs([],DOFAffine.X|DOFAffine.Y|DOFAffine.RotationAxis,[0,0,1])
        #basemanip.MoveActiveJoints(goal=[3.0,0.6,0],maxiter=5000,steplength=0.15,maxtries=2)
    waitrobot(robot)

        #robot.SetActiveDOFs([],DOFAffine.X|DOFAffine.Y|DOFAffine.RotationAxis,[0,0,1])
        #localgoal = [0,2.4,0]
        #T = robot.GetTransform()
        #goal = dot(T[0:3,0:3],localgoal) + T[0:3,3]
        #with robot:
        #    robot.SetActiveDOFValues(goal)
        #    incollision = env.CheckCollision(robot)
        #    if incollision:
        #        print 'goal in collision!!'

    #basemanip.MoveActiveJoints(goal=goal,maxiter=5000,steplength=0.15,maxtries=2)
    #waitrobot(robot)

    #raw_input('press ENTER to continue...');

    print 'move the arm to the designated position on another table to place the target down'
    Tgoal = array([[0,-1,0,3.485],[-1,0,0,1.4],[0,0,-1,1.1],[0,0,0,1]])
    res = basemanip.MoveToHandPosition(matrices=[Tgoal],seedik=16)
    waitrobot(robot)

    taskprob.ReleaseFingers(target=target)
    waitrobot(robot)

    print 'move manipulator to initial position'
    with env:
        #robot.Grab(target)
        #basemanip.MoveManipulator(zeros(len(manip.GetArmIndices())),jitter=0.04)
        basemanip.MoveManipulator(goal=[-pi/4, pi/2, pi/2, -pi/2, 0, -pi/2, 0])
    waitrobot(robot)
    #basemanip.MoveManipulator(goal=[0, 0, 1.29023451, 0, -2.32099996, 0, -0.69800004, 0])
    #waitrobot(robot)

    print 'close fingers until collision'
    taskprob.CloseFingers()
    waitrobot(robot)

    raw_input('press ENTER to continue...');

from optparse import OptionParser
from openravepy.misc import OpenRAVEGlobalArguments

@openravepy.with_destroy
def run(args=None):
    """Command-line execution of the example.

    :param args: arguments for script to parse, if not specified will use sys.argv
    """
    parser = OptionParser(description='Explicitly specify goals to get a simple navigation and manipulation demo.', usage='openrave.py --example simplemanipulation [options]')
    OpenRAVEGlobalArguments.addOptions(parser)
    parser.add_option('--planner',action="store",type='string',dest='planner',default=None,
                      help='the planner to use')
    (options, leftargs) = parser.parse_args(args=args)
    OpenRAVEGlobalArguments.parseAndCreateThreadedUser(options,main,defaultviewer=True)

if __name__ == "__main__":
    run()
