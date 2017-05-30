#include "ofApp.h"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

//--------------------------------------------------------------
void ofApp::setup() {
	// check for external webcam and use that if possible
	if (cam.listDevices().size() > 1)    cam.setDeviceID(1);
	else    cam.setDeviceID(0); 
	// frame in which the camera signal will be shown
	cam.setup(ofGetWidth(), ofGetHeight());
	ofBackground(ofColor::black);
	framerate = 60;
	/*recog = false;
	
	colorImage.allocate(320, 240);
	grayImage.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

*/

	//set framerate (speed) of the travelling pixel
	ofSetFrameRate(framerate);
	windowHeight = ofGetWindowHeight();
	windowWidth = ofGetWindowWidth();
	pixelSize = 1;
	spiralSize = pixelSize;
	spiralDirection = 0;
	startX = ceil(windowWidth / 2.0)- ceil(pixelSize / 2.0);
	startY = ceil(windowHeight / 2.0)- ceil(pixelSize / 2.0);
	x = startX;
	y = startY;
	cout << "done with setup.\n";

	maxSize = max(ofGetWindowHeight(), ofGetWindowWidth());

  file.open("text.txt");
  screen.open("screen.txt");

  //auto strings = ofSplitString(ofBufferFromFile("C:\\Users\\hamac\\Documents\\Visual Studio 2015\\Projects\\dynamic_distortion_calibrator\\bin\\screen.txt").getText(), ",\n",true,true);
  
  //ofBuffer buffer = ofBufferFromFile("C:\\Users\\hamac\\Documents\\Visual Studio 2015\\Projects\\dynamic_distortion_calibrator\\bin\\screen.txt");
  //for (auto line : buffer.getLines()) {
  //  linesOfTheFile.push_back(line);
  //}
  //
  //camHeight = linesOfTheFile.size();
  //camWidth = (ofSplitString(linesOfTheFile[0].c_str(), ",", true, true)).size();

  //strings.reserve(camHeight*camWidth);

  //for (auto it : linesOfTheFile) {
  //  vector<string> buff = ofSplitString(it.c_str(), ",", true, true);
  //  //strings.insert(strings.end(), buff.begin(), buff.end());
  //  for (auto itIt : buff) {
  //    strings.push_back(itIt.c_str());
  //  }
  //}
}

//--------------------------------------------------------------
void ofApp::update() {
	// static update of the received Signal
	cam.update();
	/*img = cam.getPixels();
	colorImage = cam.getPixels();
	colorImage.convertToGrayscalePlanarImage(grayBg, 1);*/

	//detectPixelSize(colorImage);

	
	
	// Two variantes to copy the colored image into a grayscale one
	//grayImage.setFromimg(img);
	//colorImage.convertToGrayscalePlanarImage(grayImage,1);

	//img.setImageType(OF_IMAGE_GRAYSCALE);


	/*if (!recog) {
		detectPixelSize(colorImage);
	}
	else {*/
		calculateNextSpiralPosition();
	//}


	//// Take the absolute value of the difference 
	//// between the background and incoming images.
	//grayDiff.absDiff(grayBg, grayImage);

	//// Perform an in-place thresholding of the difference image.
	//grayDiff.threshold(80);

	//// Find contours whose areas are betweeen 20 and 25000 pixels.
	//// "Find holes" is true, so we'll also get interior contours.
	//contourFinder.findContours(grayDiff, 20, 500, 10, true);

}

//--------------------------------------------------------------
// Make a white plane on which one or more Pixel(s) are travelling 
void ofApp::draw() {

	//colorImage.draw(20, 20);    // The incoming color image
	//grayImage.draw(360, 20);  // A gray version of the incoming video
	//grayBg.draw(20, 280);     // The stored background image
	//grayDiff.draw(360, 280);  // The thresholded difference image



	// draw the grayscale image in the upper left corner of the window
	//grayImage.draw(0, 0);
	

	// for debug purposes: draw the blurred and thresholded image roughly in the
	// middle of the screen; delete later
	//contourImage.draw(810, 0);

	////TODO: not quite stable yet, vulnerable to scale of square and surrounding lightning

	//contourFinder.findContours(contourImage, 1, 100, 1, false, true);
	//cout << "Found " << contourFinder.nBlobs << " blobs. \n";

	// Draw each blob individually from the blobs vector
 /* int numBlobs = contourFinder.nBlobs;
	for (int i = 0; i<numBlobs; i++) {
		contourFinder.blobs[i].draw(360, 280);
	}
*/
	// draw the travelling pixel
	ofSetColor(ofColor::white);
	ofDrawRectangle(x, y, pixelSize, pixelSize);
	ofSetColor(ofColor::black);


	img = cam.getPixels();


	int w = img.getWidth();
	int h = img.getHeight();

	float maxBrightness = 0; // these are used in the search
	int maxBrightnessX = 0; // for the brightest location
	int maxBrightnessY = 0;

	// Search through every pixel. If it is brighter than any
	// we've seen before, store its brightness and coordinates.
	// After testing every pixel, we'll know which is brightest!
	for (int y = 0; y<h; y++) {
		for (int x = 0; x<w; x++) {
			ofColor colorAtXY = img.getColor(x, y);
			float brightnessOfColorAtXY = colorAtXY.getBrightness();
			if (brightnessOfColorAtXY > maxBrightness) {
				maxBrightness = brightnessOfColorAtXY;
				maxBrightnessX = x;
				maxBrightnessY = y;
			}
			file << brightnessOfColorAtXY << " ; ";
		}
		file << endl;
	}
  file << endl << endl << "newLine" << endl << endl;

	// Draw the image.
	ofSetColor(255);
	//img.draw(0, 0);

	// Draw a circle at the brightest location.
	ofNoFill();

	ofSetColor(ofColor::red);
	ofDrawEllipse(maxBrightnessX, maxBrightnessY, 40, 40);



	ofSetColor(ofColor::black);

	
}



//--------------------------------------------------------------
void ofApp::detectPixelSize(ofxCvColorImage &image) {

	/*int width = image.getWidth();
	int height = image.getHeight();

	for (int i = 0; i < width; ) {
		for (int j = 0; j < height; ) {
			ofRect(i, j, pixelSize, pixelSize);
			j += height / 10;
		}
		i += width / 10;
	}*/


	pix = image.getPixels();
	// go over all pixels
	for (auto & pixel : pix.getPixelsIter()) {
		// and detect their colour
		pixel.getColor();
		// for debug purposes: if the colour is white something was detected
		// later: if a black pixel was recognized on a white screen
		if (!(pixel.getColor() == ofColor::black)) {
			recog = true;
			framerate = 30;
		}
		else {
			// else increase the pixel size by one
			pixelSize++;
			detectPixelSize(image);
		}
	}

	// check for pixelSize exceeding the window size
	if (pixelSize >=/* maxSize/*/10) {
		std::cout << "Error! pixelSize exceeds the size of the window!\n";
		pixelSize = ceil(/*maxSize /*/ 10.0) ;
		std::cout << "Setting pixelSize to the maxSize: " << pixelSize;
	}
	std::cout << "Found pixelSize = " << pixelSize << ".\n";
}

//--------------------------------------------------------------
void ofApp::calculateNextSpiralPosition() {
	switch (spiralDirection) {
		case 0: std::cout << "in case 0\n";
			if (y < 0) {                        // reaching upper windowborder
				x = startX - spiralSize;          // change x to left spiral side
				spiralDirection = 2;              // -> 180° turn of direction
			}
			y--;
			if (y <= startY - spiralSize) {     // dy > spiralsize will lead to 
				spiralDirection = 1;              // change of direction to left
			};
			break;
		case 1: std::cout << "in case 1\n";
			if (x < 0) {                        // reaching left windowborder
				y = startY + spiralSize;          // change x to upper spiral side
				spiralDirection = 3;              // -> 180° turn of direction
			}
			x--;
			if (x <= startX - spiralSize) {     // dx < spiralsize will lead to 
				spiralDirection = 2;              // change of direction to left
				spiralSize += pixelSize;
			};
			break;
		case 2: std::cout << "in case 2\n";
			if (y > ofGetWindowHeight()) {
				x = startX + spiralSize;
				spiralDirection = 0;
			}
			y++;
			if (y >= startY + spiralSize) {
				spiralDirection = 3; // change direction to left
			};
			break;
		case 3: std::cout << "in case 3\n";
			if (x > ofGetWindowWidth()) {
				y = startY - spiralSize;
				spiralDirection = 1;
			}
			x++;
			if (x >= startX + spiralSize) {
				spiralDirection = 0; // change direction to left
				spiralSize += pixelSize;
			};
			break;
	}
	if (x > windowWidth) {
		std::cout << "reseting spiral\n";
		x = startX;
		y = startY;
		spiralSize = pixelSize;
	}
}




