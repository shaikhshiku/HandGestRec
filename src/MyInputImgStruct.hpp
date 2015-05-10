/*
 * myInputImgStruct.hpp
 *
 *  Created on: Jan 28, 2015
 *      Author: shaiky
 */

#ifndef MYINPUTIMGSTRUCT_HPP_
#define MYINPUTIMGSTRUCT_HPP_

#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

class MyInputImgStruct{
	public:
	MyInputImgStruct();
	MyInputImgStruct(int devIndex); //overloaded constructor
	                               //to pass cam-position to videocapture object
	Mat srcLR;
	Mat src;
	Mat bw;
	vector<Mat> bwlist;
	VideoCapture vidCap; //videocapture object
	int camSrc; // will hold cam-position for external use

};
#endif /* MYINPUTIMGSTRUCT_HPP_ */
