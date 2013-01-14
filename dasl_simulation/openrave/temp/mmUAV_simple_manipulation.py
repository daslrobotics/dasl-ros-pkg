#!/usr/bin/env python
""" Test Program """

import time
import openravepy
if not __openravepy_build_doc__:
    from openravepy import *
    from numpy import *

def waitrobot(robot):
    """busy wait for robot completion"""
    while not robot.GetController().IsDone():
        time.sleep(0.01)

def main(env,options):
    "Main example code."
    # load a scene from ProjectRoom environment XML file
    env.Load('data/pr2test2.env.xml')
    time.sleep(1)

    # 1) get the 1st robot that is inside the loaded scene
    # 2) assign it to the variable named 'robot'
    robot = env.GetRobots()[0]

    manip = robot.SetActiveManipulator('leftarm_torso') # set the manipulator to leftarm + torso
    ikmodel = databases.inversekinematics.InverseKinematicsModel(robot,iktype=IkParameterization.Type.Transform6D)
    if not ikmodel.load():
        ikmodel.autogenerate()

    # create the interface for basic manipulation programs
    basemanip = interfaces.BaseManipulation(robot,plannername=options.planner)
    taskprob = interfaces.TaskManipulation(robot,plannername=options.planner)

    target=env.GetKinBody('TibitsBox1')
    with env:
        jointnames = ['l_shoulder_lift_joint','l_elbow_flex_joint','l_wrist_flex_joint','r_shoulder_lift_joint','r_elbow_flex_joint','r_wrist_flex_joint']
        robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
        basemanip.MoveActiveJoints(goal=[1.29023451,-2.32099996,-0.69800004,1.27843491,-2.32100002,-0.69799996])
    waitrobot(robot)

    print 'move robot base to target'
    with env:
        robot.SetActiveDOFs([],DOFAffine.X|DOFAffine.Y|DOFAffine.RotationAxis,[0,0,1])
        basemanip.MoveActiveJoints(goal=[2.8,-1.3,0],maxiter=5000,steplength=0.15,maxtries=2)
    waitrobot(robot)

    taskprob.ReleaseFingers()
    waitrobot(robot)

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

'''
from openravepy import *
from openravepy.misc import MultiManipIKSolver
import time, numpy

try:
    env = Environment()
    env.SetViewer('qtcoin')
    env.Load('gantry.env.xml')
    #env.Load('test1.env.xml')

    time.sleep(1)

    robot = env.GetRobots()[0]
    leftArm=robot.GetManipulator('leftarm')
    rightArm=robot.GetManipulator('rightarm')
    dualsolver = MultiManipIKSolver([leftArm,rightArm])

    manip = robot.SetActiveManipulator('leftarm') # set the manipulator to leftarm

    #robot.SetDOFValues([0],[16])

    ikmodel = databases.inversekinematics.InverseKinematicsModel(robot,iktype=IkParameterization.Type.Transform6D)
    if not ikmodel.load():
        ikmodel.autogenerate()

#    with env: # lock environment
#        Tgoal = numpy.array([[0,-1,0,-0.21],[-1,0,0,0.04],[0,0,-1,0.92],[0,0,0,1]])
#        sol = manip.FindIKSolution(Tgoal, IkFilterOptions.CheckEnvCollisions) # get collision-free solution
#        with robot: # save robot state
#            robot.SetDOFValues(sol,manip.GetArmIndices()) # set the current solution
#            Tee = manip.GetEndEffectorTransform()
#            env.UpdatePublishedBodies() # allow viewer to update new robot
#            time.sleep(10)
  
#    raveLogInfo('Tee is: '+repr(Tee))

    # create the interface for basic manipulation programs
    basemanip = interfaces.BaseManipulation(robot)
    taskprob = interfaces.TaskManipulation(robot)

#    print 'move arm to initial positions'
#    target=env.GetKinBody('TibitsBox1')
#    with env:
#        jointnames = ['l_j0','l_j1','l_j2','l_j3','l_j4','l_j5','l_j6']
#        robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
#        basemanip.MoveActiveJoints(goal=[-1.57,1.57,-1.57,0.785,0,1.57,0])
#    robot.WaitForController(0) # wait

#    print 'open fingers'
#    taskprob.ReleaseFingers()
#    robot.WaitForController(0) # wait

    #print 'move the arm to the target'
    #Tgoal = numpy.array([[0,-1,0,3.5],[-1,0,0,-2.1],[0,0,-1,1.08],[0,0,0,1]])
    #res = basemanip.MoveToHandPosition(matrices=[Tgoal],seedik=16)
    #robot.WaitForController(0) # wait

    #print 'close fingers until collision'
    #taskprob.CloseFingers()
    #robot.WaitForController(0) # wait

    #Tgoal = numpy.array([[0,-1,0,-0.23],[-1,0,0,-0.1446],[0,0,-1,0.85],[0,0,0,1]])

    #res = manipprob.MoveToHandPosition(matrices=[Tgoal],seedik=10) # call motion planner with goal joint angles
    #robot.WaitForController(0) # wait

    #taskprob = interfaces.TaskManipulation(robot) # create the interface for task manipulation programs
    #taskprob.CloseFingers() # close fingers until collision
    #robot.WaitForController(0) # wait

 #   with env:
 #       robot.Grab(env.GetKinBody('TibitsBox1'))
        # move manipulator to all zeros, set jitter to 0.04 since cup is initially colliding with table
 #       basemanip.MoveManipulator(numpy.zeros(len(manip.GetArmIndices())),jitter=0.04)
 #   robot.WaitForController(0) # wait

    #with env: # lock environment
    #    Tgoal = numpy.array([[0,-1,0,-0.21],[-1,0,0,0.04],[0,0,-1,0.92],[0,0,0,1]])
    #    sol = manip.FindIKSolution(Tgoal, IkFilterOptions.CheckEnvCollisions) # get collision-free solution
    #    with robot: # save robot state
    #        robot.SetDOFValues(sol,manip.GetArmIndices()) # set the current solution
    #        Tee = manip.GetEndEffectorTransform()
    #        env.UpdatePublishedBodies() # allow viewer to update new robot
    #        time.sleep(10)
    
    #    raveLogInfo('Tee is: '+repr(Tee))
 
#with env: # lock the environment since robot will be used
    #raveLogInfo("Robot "+robot.GetName()+" has "+repr(robot.GetDOF())+" joints with values:\n"+repr(robot.GetDOFValues()))
    #robot.SetDOFValues([0.5],[0]) # set joint 0 to value 0.5
    #T = robot.GetLinks()[1].GetTransform() # get the transform of link 1
    #raveLogInfo("The transformation of link 1 is:\n"+repr(T))

        # "Main example code."
        # load a scene from ProjectRoom environment XML file

        #env = Environment()
        #env.SetViewer('qtcoin')
        #env.Load('robots/MiniHubo/miniHubo.robot.xml')
        #env.Load('../robots/crust/openrave/smart_arm_robot.xml')
        #env.Load('mmUAV.robot.xml')
        #time.sleep(1)

        # 1) get the 1st robot that is inside the loaded scene
        # 2) assign it to the variable named 'robot'


        #manip = robot.SetActiveManipulator('arm') # set the manipulator to leftarm + torso
        #manip = robot.SetActiveManipulator('leftArm') # set the manipulator to leftarm + torso
        #ikmodel = databases.inversekinematics.InverseKinematicsModel(robot,iktype=IkParameterization.Type.Translation3D)
        #if not ikmodel.load():
        #    ikmodel.autogenerate()

        # create the interface for basic manipulation programs
        #basemanip = interfaces.BaseManipulation(robot,plannername=options.planner)
        #taskprob = interfaces.TaskManipulation(robot,plannername=options.planner)

        #target=env.GetKinBody('TibitsBox1')
        #with env:
        #    jointnames = ['gantry_x-axis_joint','gantry_y-axis_joint','gantry_z-axis_joint','gantry_yaw_joint','gantry_pitch_joint','gantry_roll_joint','lsy','lsp','lsr','lep','lem']
        #    robot.SetActiveDOFs([robot.GetJoint(name).GetDOFIndex() for name in jointnames])
        #    robot.SetActiveDOFValues([1,1,1,1,1,1,1,1,1,1,1])
            #basemanip.MoveActiveJoints(goal=[1.29023451,-2.32099996,-0.69800004,1.27843491,-2.32100002,-0.69799996])
            #waitrobot(robot)

        #print 'move robot base to target'
        #with env:
        #    robot.SetActiveDOFs([],DOFAffine.X|DOFAffine.Y|DOFAffine.RotationAxis,[0,0,1])
        #    basemanip.MoveActiveJoints(goal=[2.8,-1.3,0],maxiter=5000,steplength=0.15,maxtries=2)
        #waitrobot(robot)
finally:
    RaveDestroy()
'''
