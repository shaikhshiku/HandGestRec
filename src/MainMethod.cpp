/*
 * MainMethod.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: shaiky
 */
#include"Main.hpp"
#include"MainMethod.hpp"
#include"MyInputImgStruct.hpp"
MainMethod::MainMethod(){//constructor do nothing

}

void MainMethod::init(MyInputImgStruct *m){
	square_len=20;
	iSinceKFInit=0;
}

void MainMethod::col2OrigCol(int hsv[3], int bgr[3], Mat src){
	Mat avgBGRMat = src.clone();
		for(int i=0; i<3; i++)
		{
			avgBGRMat.data[i]=hsv[i];
		}
		cvtColor(avgBGRMat, avgBGRMat, COL2ORIGCOL);
		for(int i=0; i<3; i++)
		{
			bgr[i] = avgBGRMat.data[i];
		}
}

void MainMethod::printText(Mat src, std::string text){
	int fontFace = FONT_HERSHEY_PLAIN;
	putText(src, text, Point(src.cols/2, src.rows/10), fontFace, 1.2f, Scalar(200,0,0), 2);
}

void MainMethod::wait4PalmCover(MyInputImgStruct *m){
	m->vidCap >> m->src;
		flip(m->src,m->src,1);
		//creates NSAMPLES of ROI rectangls for sampling the color of the hand
		//**roi.push_back(Myroi(point(c,r), point(c,r), src_img))
		roi.push_back(MyROI(Point(m->src.cols/3, m->src.rows/6), Point(m->src.cols/3 + square_len, m->src.rows/6 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/4, m->src.rows/2), Point(m->src.cols/4 + square_len,m->src.rows/2 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/3, m->src.rows/1.5), Point(m->src.cols/3 + square_len,m->src.rows/1.5 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/2, m->src.rows/2), Point(m->src.cols/2 + square_len,m->src.rows/2 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/2.5, m->src.rows/2.5), Point(m->src.cols/2.5 + square_len,m->src.rows/2.5 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/2, m->src.rows/1.5), Point(m->src.cols/2 + square_len,m->src.rows/1.5 + square_len), m->src));
		roi.push_back(MyROI(Point(m->src.cols/2.5, m->src.rows/1.8), Point(m->src.cols/2.5 + square_len,m->src.rows/1.8 + square_len), m->src));

		for(int i=0; i<50; i++){
	    	m->vidCap >> m->src;
			flip(m->src, m->src, 1);
			for(int j=0; j<NSAMPLES; j++)
			{
				roi[j].drawRectangle(m->src);
			}
			string imgText=string("Cover rectangles with palm");
			printText(m->src,imgText);

			if(i==30){
			//imwrite("./images/waitforpalm1.jpg",m->src);
			}
			imshow("Main Stream", m->src);
			out << m->src;
	        if(cv::waitKey(30) >= 0) break;
		} //draws NSAMPLES of rectangles at the ROI taken with push_back() method
}
int MainMethod::getMedian(vector<int> val){
	int median;
	size_t size = val.size();
	std::sort(val.begin(), val.end());
	if (size  % 2 == 0)
	 {
		median = val[size / 2 - 1] ;
	 }
	else
	 {
		median = val[size / 2];
	 }
  return median;
}

void MainMethod::getAvgColor(MyInputImgStruct *m, MyROI roi, int avg[3]){
	Mat r;
	roi.roi_ptr.copyTo(r);
	vector<int>	hm, sm, lm;

	for(int i=2; i<r.rows-2; i++){
		for(int j=2; j<r.cols-2; j++){
			hm.push_back(r.data[r.channels() * (r.cols*i + j) + 0]);
			sm.push_back(r.data[r.channels() * (r.cols*i + j) + 1]);
			lm.push_back(r.data[r.channels() * (r.cols*i + j) + 2]);
		}
	}
	avg[0] = getMedian(hm);
	avg[1] = getMedian(sm);
	avg[2] = getMedian(lm);
}

void MainMethod::average(MyInputImgStruct *m){
	m->vidCap >> m->src;
	flip(m->src, m->src, 1);
	for(int i=0; i<30; i++)
	{
		m->vidCap >> m->src;
		flip(m->src, m->src, 1);
		cvtColor(m->src, m->src, ORIGCOL2COL);
		for(int j=0; j<NSAMPLES; j++)
		{
			getAvgColor(m, roi[j], avgColor[j]);
			roi[j].drawRectangle(m->src);
		}
		cvtColor(m->src, m->src, COL2ORIGCOL);
	string imgText = string("Evaluating Average Color of Hand...");
	printText(m->src, imgText);
	imshow("Main Stream", m->src);

	if(cv::waitKey(30) >= 0)
		break;
	}
}

void MainMethod::initTrackbars(){
	for(int i=0; i<NSAMPLES; i++)
	{
		c_lower[i][0] = 12;
		c_upper[i][0] = 7;
		c_lower[i][1] = 30;
		c_upper[i][1] = 40;
		c_lower[i][2] = 80;
		c_upper[i][2] = 80;
	}
	createTrackbar("lower1","trackbars",&c_lower[0][0],255);
	createTrackbar("lower2","trackbars",&c_lower[0][1],255);
	createTrackbar("lower3","trackbars",&c_lower[0][2],255);
	createTrackbar("upper1","trackbars",&c_upper[0][0],255);
	createTrackbar("upper2","trackbars",&c_upper[0][1],255);
	createTrackbar("upper3","trackbars",&c_upper[0][2],255);

}

void MainMethod::normalizeColors(MyInputImgStruct *m){
/***
 * DOC: copy all boundaries read from trackbars to
 * all of the different boundaries.
 * Then normalize all boundaries so that the threshhold is
 * within 0 and 255.
 */
	for( int i=1; i<NSAMPLES; i++)
	{
		for( int j=0; j<3; j++)
		{
			c_lower[i][j] = c_lower[0][j];
			c_upper[i][j] = c_upper[0][j];
		}
	}

	for(int i=0; i<NSAMPLES; i++)
	{
		if( (avgColor[i][0] - c_lower[i][0]) < 0 )
		{
			c_lower[i][0] = avgColor[i][0];
		}
		if( (avgColor[i][1]-c_lower[i][1]) < 0 )
		{
			c_lower[i][1] = avgColor[i][1];
		}
		if( (avgColor[i][2]-c_lower[i][2]) < 0 )
		{
			c_lower[i][2] = avgColor[i][2];
		}
		if( (avgColor[i][0]+c_upper[i][0]) > 255 )
		{
			c_upper[i][0] = 255-avgColor[i][0];
		}
		if( (avgColor[i][1]+c_upper[i][1]) > 255 )
		{
			c_upper[i][1] = 255-avgColor[i][1] ;
		}
		if((avgColor[i][2]+c_upper[i][2]) >255 )
		{
			c_upper[i][2] = 255-avgColor[i][2] ;
		}
	}
}

void MainMethod::produceBinaries(MyInputImgStruct *m){
	Mat foo;
	Scalar lowerBound, upperBound;
	for(int i=0; i<NSAMPLES; i++)
	{
		normalizeColors(m);
		lowerBound = Scalar( avgColor[i][0] - c_lower[i][0] , avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2] );
		upperBound=Scalar( avgColor[i][0] + c_upper[i][0] , avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2] );
		m->bwlist.push_back(Mat(m->srcLR.rows, m->srcLR.cols, CV_8U));
		inRange(m->srcLR, lowerBound, upperBound, m->bwlist[i]);
	}
	m->bwlist[0].copyTo(m->bw);
	for(int i=1; i<NSAMPLES; i++){
		m->bw += m->bwlist[i];
	}
	medianBlur(m->bw, m->bw, 7);
}

void MainMethod::initWindows(MyInputImgStruct m){
	/***
	 * DOC: creates cv::window for displaying image samples
	 * from different methods
	 */
    namedWindow("trackbars", CV_WINDOW_KEEPRATIO);
    namedWindow("Main Stream", CV_WINDOW_FULLSCREEN);
}

void MainMethod::showWindows(MyInputImgStruct m){
	vector<Mat> channels;
	Mat result;
	Rect roi( Point( 3*m.src.cols/4, 0 ), m.bw.size());

	pyrDown(m.bw,m.bw);
	pyrDown(m.bw,m.bw);

	for(int i=0;i<3;i++)
		channels.push_back(m.bw);

	merge(channels,result);
	result.copyTo( m.src(roi));
	imshow("Main Stream",m.src);
}

int MainMethod::findBiggestContour(vector<vector<Point> > contours){
	int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        if((signed int)contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}

void MainMethod::myDrawContours(MyInputImgStruct *m, MyGesture *hg){
	drawContours(m->src, hg->HullP, hg->cIdx, cv::Scalar(200,0,0), 2, 8, vector<Vec4i>(), 0, Point());
//@TODO0004:[DONE] resolve Rectangle parameters issue
	rectangle(m->src, hg->bRect.tl(), hg->bRect.br(), Scalar(0,0,200));
	vector<Vec4i>::iterator d = hg->Defects[hg->cIdx].begin();
	vector<Mat> channels;
	Mat result;

	for(int i=0; i<3; i++)
		channels.push_back(m->bw);
	merge(channels,result);
	drawContours(result,hg->contours,hg->cIdx,cv::Scalar(0,200,0),6, 8, vector<Vec4i>(), 0, Point());
	drawContours(result, hg->HullP, hg->cIdx, cv::Scalar(0,0,250), 10, 8, vector<Vec4i>(), 0, Point());
	while( d != hg->Defects[hg->cIdx].end() )
	{
   	    Vec4i& v=(*d);
	    int startidx=v[0]; Point ptStart(hg->contours[hg->cIdx][startidx] );
   		int endidx=v[1]; Point ptEnd(hg->contours[hg->cIdx][endidx] );
  	    int faridx=v[2]; Point ptFar(hg->contours[hg->cIdx][faridx] );

	    line( m->src, ptStart, ptFar, Scalar(0,255,0), 1 );
		line( m->src, ptEnd, ptFar, Scalar(0,255,0), 1 );
	   	circle( m->src, ptFar, 4, Scalar(0,255,0), 2 );
	   	circle( m->src, ptEnd, 4, Scalar(0,0,255), 2 );
	   	circle( m->src, ptStart, 4, Scalar(255,0,0), 2 );
   		circle( result, ptFar,   9, Scalar(0,205,0), 5 );
   		d++;
	}
	//	imwrite("./images/contour_defects_before_eliminate.jpg",result);
}

void MainMethod::makeContours(MyInputImgStruct *m, MyGesture *hg){
	Mat aBw;
	pyrUp(m->bw, m->bw);
	m->bw.copyTo(aBw);
	findContours(aBw, hg->contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	hg->initVectors();
	hg->cIdx = findBiggestContour(hg->contours);
	if(hg->cIdx != -1)
	{
		//approxPolyDP( Mat(hg->contours[hg->cIdx]), hg->contours[hg->cIdx], 11, true );
		hg->bRect = boundingRect(Mat(hg->contours[hg->cIdx]));
		convexHull(Mat(hg -> contours[hg -> cIdx]), hg->HullP[hg->cIdx], false, true);
		convexHull(Mat(hg -> contours[hg -> cIdx]), hg->HullI[hg->cIdx], false, false);
		approxPolyDP(Mat(hg -> HullP[hg -> cIdx]), hg->HullP[hg->cIdx], 18, true );

		if(hg->contours[hg->cIdx].size()>3 )
		{
			convexityDefects(hg->contours[hg->cIdx], hg->HullI[hg->cIdx], hg->Defects[hg->cIdx]);
			hg->eleminateDefects(m);
		}
		bool isHand = hg->detectIfHand();
		hg->printGestureInfo(m->src);
		if(isHand){
			hg->getFingerTips(m);
			hg->drawFingerTips(m);
			myDrawContours(m,hg);
		}
	}
}
