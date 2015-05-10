/*
 * MyGesture.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: shaiky
 */
#include"MyGesture.hpp"
#include"Main.hpp"

MyGesture::MyGesture(){ //constructor application
	FrameNumber = 0; //int public
	nrNoFinger = 0;
	fontFace = FONT_HERSHEY_PLAIN; //sets the font type for display text
} //ENDOF Method: printGestureInfo()

void MyGesture::printGestureInfo(Mat src){
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fontColor(245, 200, 200);
	int xpos = src.cols / 1.5;
	int ypos = src.rows / 1.6;
	float fontSize = 0.7f;
	int lineChange = 14;
	std::string info = "Figure Info:";
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fontColor);
	xpos = xpos + lineChange;
	info = std::string("bounding box height, width ") + std::string(int2String(numOfDefects));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fontColor);
	xpos = xpos + lineChange;
	info = std::string("is Hande: ") + std::string(bool2String(isHand));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fontColor);
} //ENDOF Method: printGestureInfo(Mat src)


bool MyGesture::detectIfHand(){
/***
 * @DOC: calls the analyzecontours method which sets the FingerTip
 * information to FingerTips Vector tests the FingerTips vector with
 * four conditions and SETs/RESETs the isHand variable
**/
	analyzeContours();
	double h = bRect_Width;
	double w = bRect_Height;
	isHand = true;
	if(FingerTips.size() > 5){
			isHand = false;
		} else if(h == 0 || w == 0){
			isHand = false;
		} else if((h/w > 4) || (w/h > 4)){
			isHand = false;
		} else if (bRect.x < 20){
			isHand = false;
	}
	return isHand;
} //ENDOF Method: detectIfHand()

void MyGesture::initVectors(){
/***
  * @DOC: Initializes the vectors required for convex hull calculations
  * of the hand region
 **/

	HullI = vector<vector<int> >(contours.size());
	HullP = vector<vector<Point> >(contours.size());
	Defects = vector<vector<Vec4i> >(contours.size());
} //ENDOF Method: getFingerNumbers(MyInputImgStruct *m)

void MyGesture::getFingerNumbers(MyInputImgStruct *m){
/***
 * @DOC: Calculates the most frequent finger numbers over last 20 frames
**/

	removeRedundantFingerTips();
	if(bRect.height > m->src.rows/2 && nrNoFinger > 12 && isHand){
		numberOfColor = Scalar(0, 200, 0);
		addFingerNumberTovector();
		if(FrameNumber > 20){
			nrNoFinger = 0;
			FrameNumber = 0;
			computeFingerNumbers();
			numbers2Display.push_back(mostfreqFingerNum);
			fingerNumbers.clear();
		} else {
			FrameNumber++;
		}
	} else {
		nrNoFinger++;
		numberOfColor = Scalar(200, 200, 200);
	}
	addNumberToImg(m);
} //ENDOF Method: getFingerNumbers(MyInputImgStruct *m)


void MyGesture::eleminateDefects(MyInputImgStruct *m){
/***
 * @DOC: Takes all the defect points into vector d. Loops through d and
 * measures the P2P distance between the Defect Points and decides whether
 * the Point will be dismissed or kept based on l>0.4lbb and Theta>85'
**/

	int tolerance = bRect_Height/5;
	float angleTol = 95;
	vector<Vec4i> newDefects;
	int startidx, endidx, faridx;
	vector<Vec4i>::iterator d = Defects[cIdx].begin();
	while(d != Defects[cIdx].end())
	{
		Vec4i& v = (*d);
		startidx = v[0];
		Point ptStart(contours[cIdx][startidx]);
		endidx = v[1];
		Point ptEnd(contours[cIdx][endidx]);
		faridx = v[2];
		Point ptFar(contours[cIdx][faridx]);

		if((distanceP2P(ptStart, ptFar) > tolerance) &&
				(distanceP2P(ptEnd, ptFar) > tolerance) &&
						(getAngle(ptStart, ptFar, ptEnd) < angleTol)){
			if(ptEnd.y > (bRect.y + bRect.height - bRect.height/4)) {/*Do Nothing*/}
			else if(ptStart.y > (bRect.y + bRect.height - bRect.height/4)) {/*Do Nothing*/}
			else {
				newDefects.push_back(v);
			}
		}
		d++;
	}//ENDOF While()
	numOfDefects = newDefects.size();
	Defects[cIdx].swap(newDefects);
	removeRedundantEndPoints(Defects[cIdx], m);
} //ENDOF Method: eleminateDefects(MyInputImgStruct *m)

void MyGesture::getFingerTips(MyInputImgStruct *m){
/***
 *@DOC:  Gets which of the remaining Defect points are the finger tips
 * and returns it into the vector FingerTips
**/
	FingerTips.clear();
	int i = 0;
	vector<Vec4i>::iterator d = Defects[cIdx].begin();
	while(d != Defects[cIdx].end())
	{
		Vec4i& v = (*d);
		int startidx = v[0];
		Point ptStart(contours[cIdx][startidx]);
		int endidx = v[1];
		Point ptEnd(contours[cIdx][endidx]);
		int faridx = v[2];
		Point ptFar(contours[cIdx][faridx]);
		if(i==0){
			FingerTips.push_back(ptStart);
			i++;
		}
		FingerTips.push_back(ptEnd);
		d++;
		i++;
	}//ENDOF while()
	if(FingerTips.size() == 0)
	{
		checkForOneFinger(m);
	}
} //ENDOF Method: getFingerTips(MyInputImgStruct *m)

void MyGesture::drawFingerTips(MyInputImgStruct *m){
/***
 *@DOC: draw small circle at each fingertips
 * Points are taken from FingetTips vector
**/
	Point p;
	int k=0;
	for(int i=0; i < FingerTips.size(); i++)
	{	p = FingerTips[i];
		putText(m->src, int2String(i), p-Point(0,30), fontFace, 1.2f, Scalar(200,200,200), 2);
		circle( m->src, p, 5, Scalar(100,255,100), 4 );
	}
} //ENDOF Method: drawFingerTips(MyInputImgStruct *m)

									/********ENDOF Public Method Occurrences********/

std::string MyGesture::bool2String(bool tf){
/***
 * DOC: takes a boolean parameter and returns "true" or "false" strings
 */
	if(tf)
		return "true";
	else
		return "false";
} //ENDOF Method: bool2String()

std::string MyGesture::int2String(int number){
/***
 * DOC: takes a number and returns it string form
**/
	std::string strNum;
	std::stringstream strObj;
	strObj << number;
	strNum = strObj.str();
	return strNum;
} //ENDOF Method: int2String()

float MyGesture::getAngle(Point s, Point f, Point e){
/***
 * DOC: takes 3 points and return the angle between them
**/
	float LineFS = distanceP2P(f, s);
	float LineFE = distanceP2P(f, e);
	float delTheta = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float theta = acos(delTheta / (LineFS * LineFE));
	theta = theta*180/PI;
	return theta;
} //ENDOF Method: getAngle()

float MyGesture::distanceP2P(Point a, Point b){
/***
 * DOC: gets the linear distance between point a and b
 */
	float d = sqrt(fabs( pow((a.x-b.x), 2) + pow((a.y-b.y), 2) )) ;
	return d;
} //ENDOF Method: distanceP2P()

void MyGesture::checkForOneFinger(MyInputImgStruct *m){
/***
 * DOC: checks if Defects.size()=0 and user has one finger raised
 */
	int yTol = bRect.height/6;
	Point maxP;
	maxP.y = m->src.rows;
	vector<Point>::iterator d = contours[cIdx].begin();
	while(d != contours[cIdx].end())
	{
		Point v = (*d);
		if(v.y < maxP.y)
		{
			maxP = v;
			std::cout << maxP.y << std::endl;
		}
		d++;
	}//ENDOF while()
	int n = 0;
	d = HullP[cIdx].begin();
	while(d != HullP[cIdx].end())
	{
		Point v = (*d);
		std::cout<<"(x, y) "<< v.x <<", "<<v.y<<" maxPy: "<< maxP.y <<" yTolerance: "<< yTol << std::endl;
		if((v.y < (maxP.y + yTol)) && (v.y != maxP.y) && (v.x != maxP.x))
		{
			n++;
		}
		d++;
	}//ENDOF while()
	if(n == 0)
	{
		FingerTips.push_back(maxP);
	}
} //ENDOF Method:checkForOneFinger()

void MyGesture::analyzeContours(){
/***
 * DOC: Resolve rect parameters height and width
 */
	bRect_Height = bRect.height;
	bRect_Width = bRect.width;
}//ENODF Method: analyzeContours()

void MyGesture::computeFingerNumbers(){
/***
 * DOC: calculates the figer number in the current frame
 */
	std::sort(fingerNumbers.begin(), fingerNumbers.end());
	int freqNum, thisFreqNum = 1, maxFreqNum = 1;
	freqNum = fingerNumbers[0];
	for(int i=1; i<fingerNumbers.size(); i++)
	{
		if(fingerNumbers[i-1] != fingerNumbers[i])
		{
			if(thisFreqNum > maxFreqNum)
			{
				freqNum = fingerNumbers[i-1];
				maxFreqNum = thisFreqNum;
			}
			thisFreqNum = 0;
		}
		thisFreqNum++;
	}//ENDOF for()
	if(thisFreqNum > maxFreqNum)
	{
		freqNum = fingerNumbers[fingerNumbers.size()-1];
	}
	mostfreqFingerNum = freqNum;
}//ENDOF Method: computeFingerNumbers();

void MyGesture::addNumberToImg(MyInputImgStruct *m){
/***
 * DOC: displays the number of fingers in the image
 */
	int fontFace = FONT_HERSHEY_PLAIN;
	int xPos = 10, yPos = 10, offset = 30;
	float fontSize = 1.5f;
	for(int i=0; i<numbers2Display.size(); i++)
	{
		rectangle(m->src,Point(xPos,yPos),Point(xPos+offset,yPos+offset),numberOfColor, 2);
		putText(m->src, int2String(numbers2Display[i]), Point(xPos+7, yPos+offset-3), fontFace, fontSize, numberOfColor);
		xPos = xPos+40;
		if(xPos>(m->src.cols - m->src.cols/3.2))
		{
			yPos+=40;
			xPos=10;
		}
	}
}//ENDOF method: addNumberToImg()

void MyGesture::addFingerNumberTovector(){
/***
 * DOC: adds the size of vector<Point> FingetTips to
 * vector<int> fingerNumbers using push_back()
 */
	int i = FingerTips.size();
	fingerNumbers.push_back(i);
}//ENDOF Method: addFingerNumberTovector()

void MyGesture::removeRedundantEndPoints(vector<Vec4i> newDefects, MyInputImgStruct *m){
/***
 * DOC:
 */
	Vec4i Temp;
	int startidx, endidx, faridx, startidx2, endidx2;
	float Tolerance = bRect_Width/6;
	for(int i=0; i<newDefects.size(); i++)
	{
		for(int j=i; j<newDefects.size(); j++)
		{
			startidx = newDefects[i][0];
			Point ptStart(contours[cIdx][startidx]);
			endidx = newDefects[i][1];
			Point ptEnd(contours[cIdx][endidx]);
			startidx2 = newDefects[j][0];
			Point ptStart2(contours[cIdx][startidx2]);
			endidx2 = newDefects[j][1];
			Point ptEnd2(contours[cIdx][endidx2]);
			if(distanceP2P(ptStart, ptEnd2) < Tolerance)
			{
				contours[cIdx][startidx] = ptEnd2;
				break;
			}
			if(distanceP2P(ptEnd, ptStart2) < Tolerance)
			{
				contours[cIdx][startidx2] = ptEnd;
			}
		}
	}
}//ENDOF Method: removeRedundantEndPoints(newDefects, *m)

void MyGesture::removeRedundantFingerTips(){
/***
 * DOC:
 */
	vector<Point> newFingers;
	for(int i=0; i<FingerTips.size(); i++)
	{
		for(int j=i; j<FingerTips.size(); j++)
		{
			if( (distanceP2P(FingerTips[i], FingerTips[j]) < 10) && (i != j) )
			{ /*Do Nothing*/}
			else
			{
				newFingers.push_back(FingerTips[i]);
				break;
			}
		}
	}
}//ENDOF Method: removeRedundentFingerTips()
