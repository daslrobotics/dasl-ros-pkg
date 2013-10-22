import cv2
import cv2.cv as cv
import numpy as np
import math



obj_points = [[-2,-2,0],[-2,2,0],[2,2,0],[2,-2,0]]

img_points = [[368,94],[357,326],[136,313],[145,89]]

camera_matrix=None
dist_coefs=None
capture = cv2.VideoCapture(0)
degreeOfViewHorz=60

def proccessWebcam():
    while(1):
   	 _,img = capture.read()
   	 multi = cv2.split(img)
   	 red = cv2.subtract(multi[2],(multi[1]/2))
   	 red = cv2.subtract(red,(multi[0]/2))
   	 cv2.imshow("red",red)
   	 
   	 _,red=cv2.threshold(red,-1,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
   	 gray=cv2.GaussianBlur(red,(15,15),0)
   	 gray = cv2.inRange(gray,100,255)
#   	 close = cv2.morphologyEx(gray,cv2.MORPH_CLOSE,kernel)
#   	 div = np.float32(gray)/(close)
#   	 res = np.uint8(cv2.normalize(div,div,0,255,cv2.NORM_MINMAX))
#   	 _,gray = cv2.threshold(res,-1,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
   	 cv2.imshow("res2",gray)


   	 contours,heirarchy = cv2.findContours(gray, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_TC89_KCOS)
   	 target=None
   	 highestArea=0
   	 for i in contours:
   		 tmpArea=cv2.contourArea(i)
   		 if tmpArea>highestArea and len(i)>5:
   			 target = i
   			 highestArea=tmpArea
   	 if not target==None:
   		 target = cv2.convexHull(target)
   		 target = cv2.approxPolyDP(target,10,True)
   		 cv2.drawContours(img,target,-1,(255,255,0),5)
   		 if len(target)==4:
   			 PnP_target = []
   			 for b in range(0,len(target)):
   				 PnP_target.append([target[b][0][0],target[b][0][1]])
   			 PnP_target=np.float32(PnP_target)
   			 retval,rvec,tvec = cv2.solvePnP(obj_points,PnP_target,camera_matrix,dist_coefs,np.float32([0,0,0]),np.float32([0,0,0]),False,cv2.CV_ITERATIVE)
   			 
   			 print "rvec",  rvec
   			 print "tvec",  tvec
   	 cv2.imshow("w2",img)
   	 if cv2.waitKey(5)==27:
   		 break

def calcMarker(target,height,width):
#data from an ellipse   ((245.39108276367188, 256.1669616699219), (86.41423797607422, 277.7344055175781), 91.34845733642578)
#                       	^^center point x 	^^ y              	^^secondary axis 	^^main axis     	^^angle of rotation
#y = 352*e^(-0.693*x)
    pixelsPerDegree=float(degreeOfViewHorz)/float(width)
    distance = math.pow(2,-1*target[1][1]/425+1)*11
    
    theta = math.asin(target[1][0]/target[1][1])
    phi = target[2]*3.1415926535/180
    print [distance,theta,phi]
    return [distance, theta, phi]
    

if __name__ == "__main__":
    obj_points = np.array(obj_points,'float32')
    img_points = np.array(img_points,'float32')

    w = 640
    h = 480
    size = (w,h)

    camera_matrix = np.zeros((3,3),'float32')
    camera_matrix[0,0]= 35.0   # focal length x
    camera_matrix[1,1]= 35.0   # focal length y
    camera_matrix[2,2]=1.0   	# set to 1

    camera_matrix[0,2]=w/2 	# optical center x in pixels
    camera_matrix[1,2]=h/2 	# optical center y in pixels

    dist_coefs = np.zeros(4,'float32')

    retval,camera_matrix,dist_coefs,rvecs,tvecs = cv2.calibrateCamera([obj_points],[img_points],size,camera_matrix,dist_coefs,flags=cv2.CALIB_USE_INTRINSIC_GUESS + cv2.CALIB_FIX_K3)

    print camera_matrix
    print dist_coefs

    proccessWebcam()

