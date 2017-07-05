#pragma once
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraBorder.h"
#include "commonFunctions.h"
#include "calibrationImage.h"
#include "linefinder.h"
#include "edgedetector.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<opencv2/highgui/highgui.hpp>


class CameraBorderDetector : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();

		// allows to set a pointer to a cameraArea struct outside of this class in order
		// to keep the values found by this class and set the pixelSize
		// Is actually a workaround for the lack of return values in ofBaseApp
		void setCameraAreaPointerAndPixelSize(cameraBorder *&area, int pixelSize);

		void setImageReturnVariables(calibrationImage *&vertical, calibrationImage *&horizontal);

	private:
		// pointer to the cameraArea struct used for passing things to the outside
		cameraBorder *_border;
		// Object for camera signal
		ofVideoGrabber _cam;
		// allows detection to start
		bool _beginDetection;

		// keeps track of the state in draw() function
		int _state;
		// keeps track of how often the screen was drawn
		int _drawCount;
		// counts for how many steps we didn't see the drawn pixel
		int _nonVisCount;
		// true if moving pixel was seen for the first time
		bool pxSeenOnce;
		// state for direction of movement 0 right 1 up 2 left 3 down;
		int _dirMovement;
		// size of the spiral at the moment
		int _spiralSize;

   

		pos _test;


		// last seen pixel position
		pos _lastSeenPos;

		// initial position of the Pixel
		pos _seenPix;
		
		// border detection state
		int _borderState;


		// pixelSize
		int _pixelSize;
		// x position of drawing the pixel
		//int _screen.x;
		// y position of drawing the pixel
		//int _screen.y;

		// replaces _screenX and _screenY so the max brightness that is detected for this drawn pixel could be saved to the coordinates
		pos _screen;

		// screenHeight
		int _screenHeight;
		// screenWidth
		int _screenWidth;
		// image height
		int _imageHeight;
		// image width
		int _imageWidth;



		// color image taken from video grabber
		ofxCvColorImage _color;
		// greyscale image to be worked on
		ofxCvGrayscaleImage _img;
		// pixels of image
		ofPixels _imgPixels;
		// background
		ofPixels _background;
		// difference between image and background
		ofPixels _diffPixels;
		// background set
		bool _backgroundSet;
		// image drawn as background for visualization purposes
		ofPixels _vis;
		bool _visDrawn;
		// finished finding initial spiral position
		bool _initPosFound;
		int _cumulativeX;
		int _cumulativeY;
		int _seenCount;
		int _startX;
		int _startY;
		int _binarySearchDirection; // 0 - up; 1 - down; 2 - left; 3 - right
		bool _pixelSeen, _borderDetected, _firstBinarySearchCall;
		// for binary search
		pos _lastSeen, _lastNotSeen, _drawnPos, _initPos;
		// coordinates of first border pixels in screen coordinates;
		pos _firstDown, _firstUp, _firstLeft, _firstRight;

		// color at a given pixel
		ofColor _colorAtXY;
		// brightness of color at a given pixel
		float _brightnessAtXY;
		// maximal brightness found
		float _maxBrightness;
		// x position of brightest pixel
		int _maxBrightnessX;
		// y position of brightest pixel
		int _maxBrightnessY;

		// returns true if all places in the image have been detected
		// otherwise it returns false
		//bool allPlacesSeen();
		// debug function
		void drawDebug();
		// set the currently brightest spot seen on the camera in the cameraArea
		void determineAndSetPosition();

		// pointers to return variables
		calibrationImage *_vertical, *_horizontal;
		// find initial spiral position
		void findInitialPosition();
		// creates crossed lines through the initial point
		void makeInitialCross();
		// creates horizontal lines all over the screen
		void makeHorizontalLines();
		// creates vertical lines all over the screen
		void makeVerticalLines();
		// write the information of the initial cross into the camera image
		void writeInitialCross(bool vertical);
		// write the information of the horizontal lines into the camera image
		void writeHorizontalLines();
		// write the information of the vertical lines into the camera image
		void writeVerticalLines();
		// calculates the borders from the initial cross and the two images
		void calculateBorders();
		// fills out the borders as found to check against
		void fillOutBorders();



		// find the right border of the camera frame
		// int dir: 0 - up; 1 - down; 2 - left; 3 - right
		// pos lastNotSeen: the last unseen position, has to be set the first time
		//					binarySearch is called
		// bool init: true: set lastNotSeen, false: don't set last not seen
		/*void binarySearch(int dir, pos lastNotSeen, bool init);
		// walk along the borders of the camera frame
		void borderWalker();

		void detectBorder();*/
};