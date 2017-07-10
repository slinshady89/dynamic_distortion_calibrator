#pragma once
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraArea.h"
#include "commonFunctions.h"
#include "calibrationImage.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <limits.h>


class CameraAreaDetector : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();

		// allows to set a pointer to a cameraArea struct outside of this class in order
		// to keep the values found by this class and set the pixelSize
		// Is actually a workaround for the lack of return values in ofBaseApp
		void setCameraAreaPointerAndPixelSize(cameraArea *&area, int pixelSize);
		
		// setter for spacing used in center point detection
		void setSpacing(int spacing);
		// getter for spacing used in center point detection
		int getSpacing();

		// setter for camera resolution's height
		void setResolutionHeight(int resolutionHeight);
		// getter for camera resolution's height
		int getResolutionHeight();

		// setter for camera resolution's width
		void setResolutionWidth(int resolutionWidth);
		// getter for camera resolution's width
		int getResolutionWidth();

		// setter for lower threshold for canny edge detection
		void setCannyLowerThreshold(int lowerThreshold);
		// getter for lower threshold for canny edge detection
		int getCannyLowerThreshold();

		// setter for upper threshold for canny edge detection
		void setCannyUpperThreshold(int upperThreshold);
		// getter for upper threshold for canny edge detection
		int getCannyUpperThreshold();

		// setter for jump width
		void setJump(int jump);
		// getter for jump width
		int getJump();

	private:
		// pointer to the cameraArea struct used for passing things to the outside
		cameraArea *_area;
		// Object for camera signal
		ofVideoGrabber _cam;
		// allows detection to start
		bool _beginDetection;
		// camera resolution for camera setup, has to be set
		int _resolutionWidth, _resolutionHeight;
		// thresholds for canny detector
		int _cannyLower, _cannyUpper;
		// contains the how many-th pixel is taken
		int _jump;
		// ratio, how much of the screen does the camera capture
		float _coverageRatio;

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

		// last seen pixel position
		pos _lastSeenPos;

		// initial position of the Pixel
		pos _seenPix;
		
		// border detection state
		int _borderState;


		// pixelSize
		int _pixelSize;
		

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

		// spacing between the pixels for the center point detection
		int _spacing;
		// distance from the screen borders, this is how far the rectangle gets drawn
		int _distanceFromScreenX, _distanceFromScreenY;
		// edge position, aka place where rectangle ends
		int _x, _y;
		// information about the outermost visible poxeils from findInitialPosition
		int _leastX, _largestX, _leastY, _largestY;
		// borders for the drawing to save up on frames
		int _leftBorder, _rightBorder, _upperBorder, _lowerBorder;
		// image in which the edge is saved
		ofImage _contour;
		// test ofPixels for filling out the area in one colour and the rest of the screen
		// with another
		ofPixels _test;

		int _previousX, _previousY;
		bool _firstBorderReached, _secondBorderReached;

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
		void determineBrightestPosition();

		// goes over the _contour image and writes the known position
		// at the positions of the white pixels (edge) 
		void writeLineCorrespondences(bool vert);

		// pointers to return variables
		calibrationImage *_vertical, *_horizontal;
		// find initial spiral position
		void findInitialPosition();
		// creates crossed lines through the initial point
		void makeInitialCross(bool vertical);
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
		void fillOutArea();



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