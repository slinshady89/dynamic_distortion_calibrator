#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraArea.h"
#include "commonFunctions.h"

class CameraAreaDetector : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();

		void mousePressed(int x, int y, int button);

		// allows to set a pointer to a cameraArea struct outside of this class in order
		// to keep the values found by this class and set the pixelSize
		// Is actually a workaround for the lack of return values in ofBaseApp
		void setCameraAreaPointerAndPixelSize(cameraArea *&area, int pixelSize);

	private:
		// pointer to the cameraArea struct used for passing things to the outside
		cameraArea *_area;
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
		// size of the spiral atm 
		int _spiralSize;

		int _minX, _minY, _maxX, _maxY;

		// last seen pixel position
		tuple<int, int, float> _lastSeenPos;


		// pixelSize
		int _pixelSize;
		// x position of drawing the pixel
		int _screenX;
		// y position of drawing the pixel
		int _screenY;

		// screenHeight
		int _screenHeight;
		// screenWidth
		int _screenWidth;
		// image height
		int _imageHeight;
		// image width
		int _imageWidth;



		// color image to detect pixels
		ofxCvColorImage _img;
		// pixels of image
		ofPixels _imgPixels;
		// background
		ofPixels _background;
		// difference between image and background
		ofPixels _diffPixels;
		// background set
		bool _backgroundSet;
		// image drawn as background for visualisation purposes
		ofPixels _vis;
		bool _visDrawn;
		// finished finding initial spiral position
		bool _initPos;
		int _cumulativeX;
		int _cumulativeY;
		int _seenCount;
		int _startX;
		int _startY;

		// color at a given pixel
		ofColor _colorAtXY;
		// brightness of color at a given pixel
		float _brightnessAtXY;
		// maximal brightness found
		float _maxBrightness;
		// x position of brightest pixel
		int _maxBrightnessX;
		// y position fo brightest pixel
		int _maxBrightnessY;

		// returns true if all places in the image have been detected
		// otherwise it returns false
		bool allPlacesSeen();
		// debug function
		void drawDebug();
		// set the currently brightest spot seen on the camera in the cameraArea
		void determineAndSetPosition();
		// calculate the next position for the pixel
		void calculateNextPosition();
		// find initial spiral position
		void findInitialPosition();
		// detect the right,left,upper and lower border of the seen camera frame
		void CameraAreaDetector::detectBordersOfFrame();
		// borders of the camera frame
		int minY, minX, maxY, maxX;


};