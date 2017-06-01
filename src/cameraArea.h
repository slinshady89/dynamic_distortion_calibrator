#pragma once
struct cameraArea {
	/*two array with the size of the camera image. At position (x, y) is stored
	the coordinate at which the pixel that was detected at (x, y) got drawn on
	the screen*/
	int** _imageX;
	int** _imageY;

	// global offset, aka distance from middle of screen to middle of image
	int _offsetX;
	int _offsetY;

	// actual local distortion
	int** _distortionX;
	int** _distortionY;
};