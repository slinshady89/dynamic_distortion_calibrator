#pragma once

#include "ofxOpenCv.h"

namespace blobDetector
{
	// returns the contourFinder with all found blobs after:
	// transforming: color -> greyscale
	// gaussianBlur
	// thresholding
	ofxCvContourFinder detectBlob(ofxCvColorImage &img, int thresh, int noBlobs);
};