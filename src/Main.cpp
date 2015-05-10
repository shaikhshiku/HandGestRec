/*
 * Hand Gesture Recognition[Refactored version]
 * Main.cpp
 * Created on: Jan 28, 2015
 *      Author: shaiky
 */

#include"Main.hpp"
#include"MyInputImgStruct.hpp"
#include"MyGesture.hpp"
#include"MyROI.hpp"
#include"MainMethod.hpp"

using namespace cv;

/* Global Variables  */
int fontFace = FONT_HERSHEY_PLAIN;
int square_len; // determines the length of the ROI Rectangles.
int avgColor[NSAMPLES][3] ; //avgColor[7][3]
int c_lower[NSAMPLES][3];   //c_lower[7][3]
int c_upper[NSAMPLES][3];   //c_upper[7][3]
int avgBGR[3];
int nrOfDefects;
int iSinceKFInit;

struct dim{int w; int h;}boundingDim;

Mat edges;
VideoWriter out;
MyROI roi1, roi2,roi3,roi4,roi5,roi6;
vector <MyROI> roi;
vector <KalmanFilter> kf;
vector <Mat_<float> > measurement;
MainMethod methods;

/* end global variables */

int main()
{
	//@TODO 001: Create object for video capture
	MyInputImgStruct m(0); //@TODO 001:[DONE]

	//@TODO 002: Create hand gesture object
	MyGesture mg; //@TODO 002: [DONE]
	methods.init(&m);
	m.vidCap >> m.src; //takes an image input to src matrix
	out.open("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, m.src.size(), true);
	methods.wait4PalmCover(&m);
	methods.average(&m);
	destroyWindow("Main Stream");
	methods.initWindows(m);
	methods.initTrackbars();

	while(1)
	{
		mg.FrameNumber++;
		m.vidCap >> m.src;
		flip(m.src, m.src, 1);
		pyrDown(m.src, m.srcLR);
		blur(m.srcLR, m.srcLR, Size(3,3));
		cvtColor(m.srcLR, m.srcLR, ORIGCOL2COL);
		methods.produceBinaries(&m);
		cvtColor(m.srcLR,m.srcLR,COL2ORIGCOL);
		methods.makeContours(&m, &mg);
		mg.getFingerNumbers(&m);
		methods.showWindows(m);
		out << m.src;

		if(cv::waitKey(30) == char ('q'))
			break;
	}
	destroyAllWindows();
	out.release();
	m.vidCap.release();
	destroyAllWindows();
	std::cout<<"test"<<std::endl;
	return 0;
}
