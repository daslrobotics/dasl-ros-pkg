#!/usr/bin/env python

from openravepy import *
env = Environment() # create openrave environment
env.SetViewer('qtcoin') # attach viewer (optional)
env.Load('gantry.env.xml') # load a simple scene
robot = env.GetRobots()[0] # get the first robot

with env: # lock the environment since robot will be used
    raveLogInfo("Robot "+robot.GetName()+" has "+repr(robot.GetDOF())+" joints with values:\n"+repr(robot.GetDOFValues()))
    robot.SetDOFValues([0.5],[0]) # set joint 0 to value 0.5
    T = robot.GetLinks()[1].GetTransform() # get the transform of link 1
    raveLogInfo("The transformation of link 1 is:\n"+repr(T))
