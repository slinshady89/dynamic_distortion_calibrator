#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// frame in which the camera signal will be shown
	cam.setup(320, 240);
	ofBackground(ofColor::white);
	//set framerate (speed) of the travelling pixel
	ofSetFrameRate(60);
	windowHeight = ofGetWindowHeight();
	windowWidth = ofGetWindowWidth();
	startX = floor(windowWidth / 2.0);
	startY = floor(windowHeight / 2.0);
	x = startX;
	y = startY;
	pixelSize = 1;
	spiralSize = pixelSize;
	spiralDirection = 0;

}

//--------------------------------------------------------------
void ofApp::update() {
	// stetic update of the received Signal
	cam.update();
	img = cam.getPixels();
	colorImage = cam.getPixels();

	
	

	// Two variantes to copy the colored image into a grayscale one
	//grayImage.setFromimg(img);
	colorImage.convertToGrayscalePlanarImage(grayImage,1);

	img.setImageType(OF_IMAGE_GRAYSCALE);

	// Loop to control if the drawn pixel is recognized
	for (i = 0; i <= img.getHeight(); i++) {
		for (j = 0; j <= img.getWidth(); j++) {
			pixels = grayImage.getPixels()[i*grayImage.getWidth() + j];
				
		}
	}










	pix = img.getPixels();
	for (auto & pixel : pix.getPixelsIter()) {

		pixel.getColor();
		if (!(pixel.getColor() == ofColor::white)) {
			recog = true;
		}
		else {
			//pixelSize++;
		}
		
	}
  



	if (recog) {
		// attempting to spiral pixel from middle point
		switch (spiralDirection) {
		case 0: std::cout << "in case 0\n";
			if (y < 0) {                        // reaching upper windowborder
				x = startX - spiralSize;          // change x to left spiral side
				spiralDirection = 2;              // -> 180?turn of direction
			}
			y--;
			if (y <= startY - spiralSize) {     // dy > spiralsize will lead to 
				spiralDirection = 1;              // change of direction to left
			};
			break;
		case 1: std::cout << "in case 1\n";
			if (x < 0) {                        // reaching left windowborder
				y = startY + spiralSize;          // change x to upper spiral side
				spiralDirection = 3;              // -> 180?turn of direction
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
	}
}

//--------------------------------------------------------------
// Make a white plane on which one or more Pixel(s) are travelling 
void ofApp::draw() {
	// finaly draw the camera frames 
	img.draw(0, 0);

	// get screen resolution
	// horizon = ofGetWindowWidth();
	// vert = ofGetWindowHeight();

	// output of screen resolution for testing
	//string text = "width " + to_string(horizon) + " height " + to_string(vert);
	//ofDrawBitmapStringHighlight(text,ofPoint(10,10,0.0),ofColor::white,ofColor::black );

	// draw the travelling pixel
	ofSetColor(ofColor::black);
	ofDrawRectangle(x, y, pixelSize, pixelSize);
	ofSetColor(ofColor::white);

	pixels = grayImage.getPixels()[100 * grayImage.getWidth() + 500];

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {


}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
