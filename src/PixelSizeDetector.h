#pragma once

#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"

class PixelSizeDetector : public ofBaseApp {
	public:
		// ofBaseApp's setup function
		void setup();
		// ofBaseApp's draw function, gets looped internally by ofRunApp(...)
		void draw();
		// ofBaseApp's update functin, gets looped internall by ofRunApp(...)
		// currently not actually used in this code
		void update();

		void mousePressed(int x, int y, int button);

		/*allows to set a pointer to an integer outside of this class in order
		to keep the value found by this class.
		Is actually a workaround for the lack of return values in ofBaseAppp.
		Parameters:
		*&pixelSize This is a reference to a pointer of type int*/
		void setPixelSizePointer(int *&pixelSize);

	private:
		// maximally allowed pixelSize
		int _maxPixelSize;
		// current pixelSize
		int *_pixelSize; 

		// roughly the number of squares drawn on the screen
		int _noSquaresDrawn; 
		// screenHeight
		int _screenHeight; 
		// screenWidth
		int _screenWidth; 
		// ratio of screenWidth to screenHeight
		float _ratio;
		// image height
		int _imageHeight;
		// image width
		int _imageWidth;

		// state: -1 - setup camera, 0 - draw, 1 - get image, 2 - work on image
		int _state;
		// counts how many times the same screen was drawn
		int _drawCount;
		// if the camera is setup this is turned true via mouse-click
		bool _setupDone;
		// keep track of whether the background has already been set
		bool _backgroundSet;

		// ensures that detection only starts after user read instructions
		bool _mousePressedOnce;
		// ensures that a third click does nothing
		bool _mousePressedTwice;
		// false - if camera doesn't recognize something; true -  if it does
		bool _foundPixelSize;
		// x position of brightest pixel
		int _maxBrightnessX;
		// y position fo brightest pixel
		int _maxBrightnessY;

		// Object for camera signal
		ofVideoGrabber _cam;
		// image from camera
		ofxCvColorImage _img;
		// pixel array of the image
		ofPixels _imgPixels;
		// background taken at beginning of pixel detection
		ofPixels _background;
		// difference between image and background
		ofImage _diff;
		// pixel array of the difference image
		ofPixels _diffPixels;
		// color at a given pixel
		ofColor _colorAtXY;
		// brightness of color at a given pixel
		float _brightnessAtXY;
		// maximal brightness found
		float _maxBrightness;
		// counts the times the same pixelSize was encountered to validate it
		int _foundPixelSizeCounter;
		// oldPixelSize
		int _oldPixelSize;

		// function to detect the brightness in the image
		void detectBrightness();
		// subtract the background from the image to avoid detection ofreflections
		void subtractBackground();
		// function that draws roughly the amount of _noSquaresDrawn on the screen
		void drawRectangles();
		// draws the current camera frame, the set background, & the remaining differenece
		// in the upper left corner if _debug is set true
		void drawDebug();
};