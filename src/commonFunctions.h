#pragma once

#include "ofxOpenCv.h"
#include "Constants.h"

namespace commonFunctions {
	// function that subtracts the background background from the image img
	// input:	ofPixels img: the current image
	//			ofPixels bkgrnd: the background you wish to subtract
	// 
	// output: ofPixels object containing the difference
	ofPixels subtractBackground(ofPixels img, ofPixels bkgrnd);

	// function to detect the brightness in the image
	// input:	ofPixels image - the image in which you want to detect the brightest pixel
	// output:	tuple<int, int, float>:
	//				- first value : x position of brightest pixel
	//				- second value: y position of brightest pixel
	//				- third value : maximal brightness value in image
	_pos detectBrightness(ofPixels img);
}