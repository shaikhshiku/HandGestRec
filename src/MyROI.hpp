/*
 * MyROI.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: shaiky
 */

#ifndef MYROI_HPP_
#define MYROI_HPP_

#include"Main.hpp"
using namespace cv;

class MyROI{
	public:
		int border_thickness;
		Scalar color;
		Mat roi_ptr; //contains the small rectangles for color sampling from hand region
		Point upper_corner, lower_corner;

		MyROI();
		MyROI(Point u_corner, Point l_corner, Mat src);
		void drawRectangle(Mat src);
};

#endif /* MYROI_HPP_ */
