/*
 * MainMethod.hpp
 *
 *  Created on: Feb 1, 2015
 *      Author: shaiky
 */

#ifndef MAINMETHOD_HPP_
#define MAINMETHOD_HPP_

#include"Main.hpp"
#include"MyROI.hpp"
#include"MyGesture.hpp"
#include"MyInputImgStruct.hpp"

class MainMethod{
	public:
	int fontFace = FONT_HERSHEY_PLAIN;
	int square_len; // determines the length of the ROI Rectangles.
	int avgColor[NSAMPLES][3] ; //avgColor[7][3]
	int c_lower[NSAMPLES][3];   //c_lower[7][3]
	int c_upper[NSAMPLES][3];   //c_upper[7][3]
	int avgBGR[3];
	int nrOfDefects;
	int iSinceKFInit;
	Mat edges;
	VideoWriter out;
	MyROI roi1, roi2,roi3,roi4,roi5,roi6;
	vector <MyROI> roi;
	vector <KalmanFilter> kf;
	vector <Mat_<float> > measurement;

	MainMethod(); //constructor
	void init(MyInputImgStruct *m);
	void col2OrigCol(int hsv[3], int bgr[3], Mat src);
	void printText(Mat src, std::string text);
	void wait4PalmCover(MyInputImgStruct *m);
	int getMedian(vector<int> val);
	void getAvgColor(MyInputImgStruct *m, MyROI roi, int avg[3]);
	void average(MyInputImgStruct *m);
	void initTrackbars();
	void normalizeColors(MyInputImgStruct *m);
	void produceBinaries(MyInputImgStruct *m);
	void initWindows(MyInputImgStruct m);
	void showWindows(MyInputImgStruct m);
	int findBiggestContour(vector<vector<Point> > contours);
	void myDrawContours(MyInputImgStruct *m, MyGesture *hg);
	void makeContours(MyInputImgStruct *m, MyGesture *hg);
};


#endif /* MAINMETHOD_HPP_ */
