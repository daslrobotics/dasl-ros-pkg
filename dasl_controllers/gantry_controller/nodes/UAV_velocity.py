#This class implements the control loop for the gantry emulation of UAV aircraft
#
#######	Example	#########
#
#UAV=UAV_velocity(1,0.01,0,5,-5)
#UAV.setPoint(0.0)
#while True:
#     velocity = UAV.update(current_speed,pitch,Fdist)
#
#


class UAV_velocity:
	"""
	Discrete PID control
	"""

	def __init__(self, P=2.0, M=1.0, Integrator=0, Integrator_max=500, Integrator_min=-500):

		self.Kp=P
		self.M=M
		self.Integrator=Integrator
		self.Integrator_max=Integrator_max
		self.Integrator_min=Integrator_min
		self.PitchOutput = 0.0
		self.set_point=0.0
		self.error=0.0

	def update(self,current_speed,pitch,Fdist):
		"""
		Calculate velocity output for given Angle feedbacks and Torque disturbance
		"""

		self.error = self.set_point - current_speed
		self.PitchOutput=self.Kp * self.error
		self.A_value = pitch * 9.81 / 4.0 / 2.5
		self.Integrator = self.Integrator + self.A_value - Fdist/self.M

		if self.Integrator > self.Integrator_max:
			self.Integrator = self.Integrator_max
		elif self.Integrator < self.Integrator_min:
			self.Integrator = self.Integrator_min

		velocity = self.Integrator 

		return velocity

	def setPoint(self,set_point):
		"""
		Initilize the setpoint of PID
		"""
		self.set_point = set_point
		self.Integrator=0.0

	def setIntegrator(self, Integrator):
		self.Integrator = Integrator

	def setKp(self,P):
		self.Kp=P

	def setKi(self,I):
		self.Ki=I
		
	def getPitchCmnd(self):
		return self.PitchOutput

	def getPoint(self):
		return self.set_point

	def getError(self):
		return self.error

	def getIntegrator(self):
		return self.Integrator