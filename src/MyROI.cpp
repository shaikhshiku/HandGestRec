/*
 * MyROI.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: shaiky
 */

#include"MyROI.hpp"
#include"Main.hpp"
using namespace cv;

MyROI::MyROI(){
	upper_corner = Point(0, 0);
	lower_corner = Point(0, 0);
}

MyROI::MyROI(Point u_corner, Point l_corner, Mat src){
	upper_corner = u_corner;
	lower_corner = l_corner;
	color = Scalar(0,255,0);
	border_thickness = 2;
	//roi_ptr contains the small sampled region from hand palm
	roi_ptr = src(Rect(u_corner.x, u_corner.y, (l_corner.x - u_corner.x), (l_corner.y - u_corner.y)) );
}

void MyROI::drawRectangle(Mat src){
	rectangle(src,upper_corner,lower_corner,color,border_thickness);
}//ENDOF Method: drawRectangle()
