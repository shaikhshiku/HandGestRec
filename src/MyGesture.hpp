/*
 * MyGesture.hpp
 *
 *  Created on: Jan 28, 2015
 *      Author: shaiky
 */

#ifndef MYGESTURE_HPP_
#define MYGESTURE_HPP_

#include"Main.hpp"
#include"myInputImgStruct.hpp"

using namespace cv;

class MyGesture{
	public:
		bool isHand;
		int cIdx, FrameNumber, mostfreqFingerNum, numOfDefects;
		double bRect_Width, bRect_Height;

		Rect rect, bRect;
		MyInputImgStruct m;
		Vector<Point> FingerTips;
		vector<vector<Point> > contours;
		vector<vector<int> > HullI;
		vector<vector<Point> >HullP;
		vector<vector<Vec4i> > Defects;

		MyGesture(); //Constructor
		void printGestureInfo(Mat src);
		bool detectIfHand();
		void initVectors();
		void getFingerNumbers(MyInputImgStruct *m);
		void eleminateDefects(MyInputImgStruct *m);
		void getFingerTips(MyInputImgStruct *m);
		void drawFingerTips(MyInputImgStruct *m);

	private:
		int fontFace, prevNrFingerTips, nrNoFinger;
		Scalar numberOfColor;
		vector<int> fingerNumbers, numbers2Display;

		string bool2String(bool tf);
		string int2String(int number);
		float getAngle(Point s, Point f, Point e);
		float distanceP2P(Point a, Point b);
		void checkForOneFinger(MyInputImgStruct *m);
		void analyzeContours();
		void computeFingerNumbers();
		void drawNewNumber(MyInputImgStruct *m);//@TODO003: My need to write occurence for this
		void addNumberToImg(MyInputImgStruct *m);
		void addFingerNumberTovector();
		void removeRedundantEndPoints(vector<Vec4i> newDefects, MyInputImgStruct *m);
		void removeRedundantFingerTips();

}; //EOFClass MyGesture


#endif /* MYGESTURE_HPP_ */
