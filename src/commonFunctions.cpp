#include "commonFunctions.h"

//_____________________________________________________________________________
ofPixels commonFunctions::subtractBackground(ofPixels img, ofPixels bkgd)
{
	ofColor color;
	// set difference image
	ofPixels diff;
	diff = img;

	// get width and height from image
	int width = img.getWidth();
	int height = img.getHeight();

	// iterate over image and subtract the background
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			color = img.getColor(x, y) - bkgd.getColor(x, y);
			diff.setColor(x, y, color);
		}
	}

	// return difference between images
	return diff;
}

//_____________________________________________________________________________
tuple<int, int, int> commonFunctions::detectBrightness(ofPixels img)
{
	// coordinates of brightest pixel
	tuple<int, int, int> brightCoord;

	// get width and height from image
	int width = img.getWidth();
	int height = img.getHeight();

	ofColor color;

	float brightness;
	float maxBrightness = 0;

	// iterate and find brightest pixel
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			color = img.getColor(x, y);
			brightness = color.getBrightness();
			if (brightness > maxBrightness) {
				maxBrightness = brightness;
				brightCoord = std::make_tuple(x, y, maxBrightness);
				std::cout << "maxBrightness: " << maxBrightness << "\n";
			}
		}
	}
	return brightCoord;
}