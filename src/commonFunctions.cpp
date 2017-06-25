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
 pos commonFunctions::detectBrightness(ofPixels img)
{
	// coordinates of brightest pixel
	 pos brightCoord;

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
      //brightness = color.getLightness();  // maybe useful for further experiments -> check definition of functions
			if (brightness > maxBrightness) {
				maxBrightness = brightness;
        brightCoord.x = x;
        brightCoord.y = y;
        brightCoord.b = maxBrightness;

				//brightCoord = std::make_tuple(x, y, maxBrightness);
				//std::cout << "maxBrightness: " << maxBrightness << "\n";
			}
		}
	}
	return brightCoord;
}


 //_____________________________________________________________________________
 pos commonFunctions::detectGrayValue(ofPixels img) {

	 // coordinates of brightest pixel
	 pos brightCoord;


	 // get width and height from image
	 int width = img.getWidth();
	 int height = img.getHeight();
	 int grayValue = 0;
	 int maxGrayValue = 0;
	 unsigned char* pixelData = img.getPixels();

	 for (size_t x = 0; x < width; x++)
	 {
		 for (size_t y = 0; y< height; y++)
		 {
			 grayValue = pixelData[img.getPixelIndex(x, y)];
			 if (grayValue>maxGrayValue)
			 {
				 brightCoord.x = x;
				 brightCoord.y = y;
				 brightCoord.b = grayValue;
         maxGrayValue = grayValue;
			 }
		 }
	 }
	 return brightCoord;
 }
