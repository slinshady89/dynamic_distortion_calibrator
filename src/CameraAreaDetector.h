#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofMain.h"
#include "cameraArea.h"

class CameraAreaDetector : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();

		void mousePressed(int x, int y, int button);

		/*allows to set a pointer to a cameraArea struct outside of this class in order
		to keep the values found by this class and set the pixelSize
		Is actually a workaround for the lack of return values in ofBaseApp*/
		void setCameraAreaPointerAndPixelSize(cameraArea *&area, int pixelSize);

	private:
		// pointer to the cameraArea struct used for passing things to the outside
		cameraArea *_area;
		// Object for camera signal
		ofVideoGrabber _cam;
		// allows detection to start
		bool _beginDetection;

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
		// image drawn as background for visualisation purposes
		ofImage _vis;

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

		// function to detect the brightness in the image
		void detectBrightness();
		// returns true if all places in the image have been detected
		// otherwise it returns false
		bool allPlacesSeen();
};