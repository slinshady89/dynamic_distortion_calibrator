#pragma once

#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"

class PixelSizeDetector : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();

		void mousePressed(int x, int y, int button);

		/*allows to set a pointer to an integer outside of this class in order
		to keep the value found by this class
		Is actually a workaround for the lack of return values in ofBaseApp*/
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

		// ensures that detection only starts after the screen is drawn
		bool _beginDetection;
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
		// color at a given pixel
		ofColor _colorAtXY;
		// brightness of color at a given pixel
		float _brightnessAtXY;
		// maximal brightness found
		float _maxBrightness;

		// function to detect the brightness in the image
		void detectBrightness();
		// subtract the background from the image to avoid detection ofreflections
		void subtractBackground();
};