#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		

		//ofxPanel gui;
		ofParameter<int> x, y;


	private:
		int pixelSize, windowHeight, windowWidth, spiralSize, startX, startY, i, j,maxSize;
    int framerate;
		int spiralDirection; // 0 -> up; 1 -> left; 2 -> down; 3 -> right
		bool recog; // recognized a rectangle in the image
		ofVideoGrabber cam;
		ofxCvContourFinder contourFinder;
		ofxCvColorImage colorImage; // color image as seen by camera
		ofxCvGrayscaleImage contourImage, grayImage, grayBg, grayDiff;
		ofxCvBlob blob;
		// Object for camera signal
		ofImage img;
    ofPixels pix;
		// detects the minimal size a square must have to be seen by the camera
		void detectPixelSize(ofxCvColorImage &image);
		// calculates the next x & y values for the spiral
		void calculateNextSpiralPosition(); 


    // Replace this ofImage with live video, eventually
    ofImage laserTagImage;

    vector<string> strings;
    vector < string > linesOfTheFile;
    ofstream file;
    ifstream screen;

    int camHeight, camWidth;
};
