#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>
#include <std_msgs/Float64.h>

#define Pi 3.14159265359

namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";
static const char WINDOW2[] = "Image window temp";
using namespace std;
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  int thresh;
  double F, CameraCalibMat[3][3];
  std_msgs::Float64 JointA1Controller;

public:
  cv::Mat src_HSV;
  cv::Mat src_RGB;

  /* function AverageLine */
  void lineAverage(cv::vector<cv::Vec2f> lines, cv::Mat& src)
  {
	  float rho=0,theta=0;
	  for( size_t i = 0; i < lines.size(); i++ )
	  		{
	  		 rho += lines[i][0];theta += lines[i][1];
	  		}
	 rho/=lines.size();theta/=lines.size();
	 cv::Point pt1, pt2;
	 double a = cos(theta), b = sin(theta);
	 double x0 = a*rho, y0 = b*rho;
	 pt1.x = cvRound(x0 + 1000*(-b));
	 pt1.y = cvRound(y0 + 1000*(a));
	 pt2.x = cvRound(x0 - 1000*(-b));
	 pt2.y = cvRound(y0 - 1000*(a));
	 //cv::line( src, pt1, pt2, cv::Scalar(80,10,55), 3, CV_AA);
	 float rho1=0,rho2=0,theta1=0,theta2=0;
	 float i1=0,i2=0;
	 for( size_t i = 0; i < lines.size(); i++ )
		{
		 if (lines[i][1]>theta)
		 {	 rho1 += lines[i][0];theta1 += lines[i][1];i1++;}
		 else
		 {   rho2 += lines[i][0];theta2 += lines[i][1];i2++;}
		}
	 rho1/=i1;theta1/=i1;
	 a = cos(theta1), b = sin(theta1);
	 x0 = a*rho1, y0 = b*rho1;
	 pt1.x = cvRound(x0 + 1000*(-b));
	 pt1.y = cvRound(y0 + 1000*(a));
	 pt2.x = cvRound(x0 - 1000*(-b));
	 pt2.y = cvRound(y0 - 1000*(a));
	 cv::line( src, pt1, pt2, cv::Scalar(0,100,255), 3, CV_AA);
	 rho2/=i2;theta2/=i2;
	 a = cos(theta2), b = sin(theta2);
	 x0 = a*rho2, y0 = b*rho2;
	 pt1.x = cvRound(x0 + 1000*(-b));
	 pt1.y = cvRound(y0 + 1000*(a));
	 pt2.x = cvRound(x0 - 1000*(-b));
	 pt2.y = cvRound(y0 - 1000*(a));
	 cv::line( src, pt1, pt2, cv::Scalar(0,100,0), 3, CV_AA);
  }

  /** @function Dilation */
  cv::Mat Dilation( const cv::Mat& src, int dilation_elem, int dilation_size)
  {
		cv::Mat dilation_dst;
		int dilation_type=1;
		if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
		else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
		else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }

		cv::Mat element = cv::getStructuringElement( dilation_type,
											 cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
											 cv::Point( dilation_size, dilation_size ) );
		/// Apply the dilation operation
		cv::dilate( src, dilation_dst, element );
		return dilation_dst;
  }

  static void onMouse( int event, int x, int y, int flags, void* param)
  {
    if( event != cv::EVENT_LBUTTONDOWN )
    	return;
    cv::Mat* rgb = (cv::Mat*) param;
    int H = (int)(*rgb).at<cv::Vec3b>(y, x)[0];
    int S = (int)(*rgb).at<cv::Vec3b>(y, x)[1];
    int V = (int)(*rgb).at<cv::Vec3b>(y, x)[2];
//    cv::setTrackbarPos(" R0: ",WINDOW,H);
//    cv::setTrackbarPos(" G0: ",WINDOW,S);
//    cv::setTrackbarPos(" B0: ",WINDOW,V);
  }

  cv::Mat redFilter(const cv::Mat& src)
  {
      assert(src.type() == CV_8UC3);

      cv::Mat redOnly;
      cv::inRange(src, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 255), redOnly);
      return redOnly;
  }
  cv::Mat redFilter(const cv::Mat& src, cv::Scalar FiltStart, cv::Scalar FiltEnd)
    {
        assert(src.type() == CV_8UC3);

        cv::Mat redOnly;
        cv::inRange(src, FiltStart, FiltEnd, redOnly);

        return redOnly;
    }
  ImageConverter()
    : it_(nh_)
  {

    image_pub_ = it_.advertise("out", 1);
    image_sub_ = it_.subscribe("/camera/image_raw", 1, &ImageConverter::imageCb, this);

    cv::namedWindow(WINDOW);
    int temp=10;
    cv::createTrackbar( " TresholdEllipse ", WINDOW, &temp, 255, NULL);
//    cv::createTrackbar( " R0: ", WINDOW, &temp, 255, NULL);
//    temp=170;
//    cv::createTrackbar( " G0: ", WINDOW, &temp, 255, NULL);
//    temp=170;
//	cv::createTrackbar( " B0: ", WINDOW, &temp, 255, NULL);
//	temp=55;
//	cv::createTrackbar( " R1: ", WINDOW, &temp, 255, NULL);
//	temp=255;
//	cv::createTrackbar( " G1: ", WINDOW, &temp, 255, NULL);
//	temp=255;
//	cv::createTrackbar( " B1: ", WINDOW, &temp, 255, NULL);
	temp=150;
	cv::createTrackbar(" Threshold:",WINDOW,&temp,400,NULL);
	temp=4;
	cv::createTrackbar(" Dilate:",WINDOW,&temp,10,NULL);
	CameraCalibMat[0][2]=640;
    CameraCalibMat[1][2]=480;

    cv::setMouseCallback(WINDOW,ImageConverter::onMouse,&src_HSV);
    cv::namedWindow(WINDOW2);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
	cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    cv::Mat src_gray(cv_ptr->image.rows,cv_ptr->image.cols,CV_8UC1);
    int R0=1;//cv::getTrackbarPos(" R0: ", WINDOW);
    int R1=55;//cv::getTrackbarPos(" R1: ", WINDOW);
    int G0=170;//cv::getTrackbarPos(" G0: ", WINDOW);
    int G1=255;//cv::getTrackbarPos(" G1: ", WINDOW);
    int B0=170;//cv::getTrackbarPos(" B0: ", WINDOW);
    int B1=255;//cv::getTrackbarPos(" B1: ", WINDOW);
    int Dilate_size = cv::getTrackbarPos(" Dilate:", WINDOW);
    src_HSV=cv::Mat(cv_ptr->image.rows,cv_ptr->image.cols,CV_8UC3);
    cv::cvtColor(cv_ptr->image, src_HSV, CV_BGR2HSV);
    src_gray=redFilter(src_HSV, cv::Scalar(R0,G0,B0),cv::Scalar(R1,G1,B1));
	//int from_to[] = { 2,0 };
	cv::blur( src_gray, src_gray, cv::Size(3,3) );
	src_gray=this->Dilation(src_gray,0,Dilate_size);
    /// Detect edges using Threshold
    cv::Mat threshold_output;
    thresh=cv::getTrackbarPos(" Threshold:", WINDOW);
    int ellipse_tresh = cv::getTrackbarPos(" TresholdEllipse ", WINDOW);
    cv::threshold( src_gray, threshold_output, ellipse_tresh, 255, cv::THRESH_BINARY );
    //cv::Canny( src_gray, threshold_output, 10, 10*2, 3 );
    /// Find contours
    cv::vector<cv::vector<cv::Point> > contours;
    cv::vector<cv::Vec4i> hierarchy;
    //threshold_output=this->Dilation(threshold_output,0,Dilate_size);
    cv::findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
//    unsigned int max_count=0;
//    unsigned int max_i =0;
    unsigned int largestContouri=0;
    double maxArea = 0;
    if(contours.size()>0)
    {
		for( unsigned int i = 0; i < contours.size(); i++ )
		{
//			size_t count = contours[i].size();
//			if( count < 20 )
//				continue;
//			if (count >= 20 && count > max_count)
//			{
//				max_count=count;
//				max_i=i;
//			}
			double area = cv::contourArea(contours[i]);
			    if (area > maxArea) {
			        maxArea = area;
			        largestContouri = i;//max_i
			    }
		}
		if (contours[largestContouri].size() >= 20)
		{
			cv::RotatedRect box=cv::fitEllipse(cv::Mat(contours[largestContouri]));
			cv::ellipse(cv_ptr->image, box, cv::Scalar(0,255,255), 1, CV_AA);
			cv::Point2f rect_points[4]; box.points( rect_points );
			for( int j = 0; j < 4; j++ )
			cv::line( cv_ptr->image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(255,0,255), 1, 8 );
			cv::string str_buffer;
			float roi_size = (box.size.height<box.size.width) ? 0.95*box.size.height:0.95*box.size.width;
			float roi_x0 = (box.center.x-roi_size/2>0)?box.center.x-roi_size/2:1;
			float roi_y0 = (box.center.y-roi_size/2>0)?box.center.y-roi_size/2:1;
			float roi_dx = (box.center.x+roi_size>threshold_output.cols)?threshold_output.cols-roi_x0:roi_size;
			float roi_dy = (box.center.y+roi_size>threshold_output.rows)?threshold_output.rows-roi_y0:roi_size;
		    //cv::Mat img_ROI = threshold_output(cv::Range( cvRound(box.center.x-rect_max), cvRound(box.center.y-rect_max)), cv::Range(cvRound(box.center.x+rect_max), cvRound(box.center.y+rect_max)));
			//cv::Rect myROI(box.center.x,box.center.y,rect_max,rect_max);
			cv::Rect myROI(roi_x0,roi_y0,roi_dx,roi_dy);
			cv::Mat mask = cv::Mat::zeros(threshold_output.rows,threshold_output.cols,CV_8UC1);
			cv::Mat roi_mask=mask(myROI);
			roi_mask=cv::Scalar(255);
			//cv::inRange(roi_mask,cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0),roi_mask);
			//cv::Rect myROI(10,50,30,24);
			//cv::Mat img_ROI = threshold_output();
			cv::Canny( threshold_output, threshold_output, 5, 5*3, 3 );
			cv::multiply(threshold_output,mask,threshold_output);
			//cv::Mat img_ROI = src_gray(myROI);
			//cv::inRange(img_ROI, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), img_ROI);
			cv::vector<cv::Vec2f> lines;
			//cvNot(&myROI, &myROI);
			//cv::HoughLines(threshold_output, lines, 1, CV_PI/180, thresh*20, 0, 0 );
			cv::HoughLines(threshold_output, lines, 1, CV_PI/180, thresh, 0, 0 );
			roi_size=lines.size();
			for( size_t i = 0; i < lines.size(); i++ )
			{
				 float rho = lines[i][0], theta = lines[i][1];
				 cv::Point pt1, pt2;
				 double a = cos(theta), b = sin(theta);
				 double x0 = a*rho, y0 = b*rho;
				 pt1.x = cvRound(x0 + 1000*(-b));
				 pt1.y = cvRound(y0 + 1000*(a));
				 pt2.x = cvRound(x0 - 1000*(-b));
				 pt2.y = cvRound(y0 - 1000*(a));
				 cv::line( cv_ptr->image, pt1, pt2, cv::Scalar(180,100,55), 3, CV_AA);
			}

			this->lineAverage(lines,cv_ptr->image);
			str_buffer=cv::format("Height = %5.2f -|- Width = %5.2f",box.size.height/2,box.size.width/2);
			cv::putText(cv_ptr->image,str_buffer,cv::Point(10,10),cv::FONT_HERSHEY_PLAIN,0.8,cv::Scalar(255,255,255),1,8);
			str_buffer=cv::format("  X    = %5.2f -|-    Y  = %5.2f",box.center.x-320,-box.center.y+240);
			cv::putText(cv_ptr->image,str_buffer,cv::Point(10,20),cv::FONT_HERSHEY_PLAIN,0.8,cv::Scalar(255,255,255),1,8);
			str_buffer=cv::format("Angle  = %5.2f  %5.2f  %5.2f  %5.2f %5.2f",roi_x0,roi_y0,roi_dx,roi_dy,roi_size);//cv::contourArea(contours[largestContouri]));//box.angle);
			cv::putText(cv_ptr->image,str_buffer,cv::Point(10,30),cv::FONT_HERSHEY_PLAIN,0.8,cv::Scalar(255,255,255),1,8);
		}
    }
    cv::waitKey(3);
    cv::imshow(WINDOW, cv_ptr->image);
    //cv::imshow(WINDOW,src_gray);//threshold_output
    cv::imshow(WINDOW2,threshold_output);
    //cv::imshow(WINDOW,mask);
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  //void cv::setMouseCallback(WINDOW,onMouse,NULL);
  ros::spin();
  return 0;
}
