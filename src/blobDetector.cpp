#include "blobDetector.h"
#include "ofxOpenCv.h"

namespace blobDetector {
	ofxCvContourFinder detectBlob(ofxCvColorImage &img, int thresh, int noBlobs) {
		// declare all needed variables
		ofxCvGrayscaleImage contourImg;
		ofxCvContourFinder contourFinder;
		int min = 1; // arbitrarily chosen, though with some thought: camera should detect only one pixel
		int max = 5;

		// first convert to grayScale
		img.convertToGrayscalePlanarImage(contourImg, 1);

		// then blur with Gaussian to get a better result for blob detection
		contourImg.blurGaussian();

		// threshold the image (might not be necessary afterall) & invert greyscale
		contourImg.threshold(thresh, true);

		// finally detect the blobs in the image
		contourFinder.findContours(contourImg, min, max, noBlobs, false, true);
		cout << "Found " << contourFinder.nBlobs << " blobs. \n";
		return contourFinder;
	}
}