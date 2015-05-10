/*
 * myInputImgStruct.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: shaiky
 */
#include"MyInputImgStruct.hpp"

MyInputImgStruct::MyInputImgStruct(){
	//do nothing
}
MyInputImgStruct::MyInputImgStruct(int devIndex){
	camSrc = devIndex; //holds webcam position for external use
	vidCap = VideoCapture(devIndex); // creates video capture object
	                                //from webcam position devIndex
}
