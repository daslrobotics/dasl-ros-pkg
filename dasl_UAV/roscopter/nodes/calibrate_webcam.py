import cv2
import cv2.cv as cv
import numpy as np
import math


capture = cv2.VideoCapture(0)

dims=(4,6)

cameraMatrix=[[1,0,0],[0,1,0],[0,0,0]]
square_size=1.0
pattern_points = np.zeros( (np.prod(dims),3),np.float32)
pattern_points[:,:2]=np.indices(dims).T.reshape(-1,2)
pattern_points*=square_size

obj_points = []
img_points = []

h,w=0,0
while True:
	_,img = capture.read()
	gray=cv2.cvtColor(img,cv2.cv.CV_BGR2GRAY)
	cv2.imshow("raw",img)
	if cv2.waitKey(500)==27:
		break
	
	square_size = 1.0
	
	h,w=img.shape[:2]
	found, corners = cv2.findChessboardCorners(gray,dims)
	if found:
		cv2.drawChessboardCorners(img,dims,corners,found)
		term = ( cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_COUNT,30,0.1)
		cv2.cornerSubPix(img,corners,(5,5),(-1,-1),term)
	if not found:
		print 'chessboard not found'
		continue
	img_points.append(corners.reshape(-1,2))
	obj_points.append(pattern_points(-1,2))
	cv2.imshow("chessboard",img)
	print 'next'

rms, camera_matrix, dist_coefs,rvecs,tvecs = cv2.calibrateCamera(obj_points, img_points, (w,h), None, None)
print "RMS:",rms
print "Camera Matrix: \n"
print "distortion coefficients: ", dist_coefs.ravel()
cv2.destroyAllWindows()



