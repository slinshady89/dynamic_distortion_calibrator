#pragma once

#include "ofxOpenCv.h"

namespace commonFunctions {
	// function that subtracts the background bkkgrnd from the image img
	ofPixels subtractBackground(ofPixels img, ofPixels bkgrnd);

	// function to detect the brightness in the image
	tuple<int, int, int> detectBrightness(ofPixels img);
}