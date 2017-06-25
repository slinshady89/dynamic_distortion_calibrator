#include "Constants.h"

struct calibrationImage {
	// contains the ground truth image, aka the screen content
	pos** groundTruth;

	// contains the image seen by the camera
	pos** image;

	// sizes of groundTruth and image
	int gtWidth;
	int gtHeight;
	int imWidth;
	int imHeight;
};