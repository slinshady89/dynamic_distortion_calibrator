#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		// Object for camera signal
		ofVideoGrabber cam;
		ofxCvColorImage colorImage; // color image as seen by camera
		ofxCvGrayscaleImage grayImage; // grayscale image from camera
		ofImage img;
		ofPixels pix;

		//ofxPanel gui;
		ofParameter<int> x, y;


	private:
		int pixelSize, windowHeight, windowWidth, spiralSize, startX, startY, i, j;
		int spiralDirection; // 0 -> up; 1 -> left; 2 -> down; 3 -> right
		bool recog; // recognized a rectangle in the image
		unsigned char pixels;
		ofxCvContourFinder contourFinder;

		// detects the minimal size a square must have to be seen by the camera
		void detectPixelSize(); 
};
