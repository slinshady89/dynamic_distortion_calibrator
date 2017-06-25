#include "cameraBorder.h"
#include "PixelSizeDetector.h"
#include "CameraBorderDetector.h"
#include "ImageCreator.h"
#include "Constants.h"


class DynamicDistortionCalibrator {
	public:
		// constructor
		DynamicDistortionCalibrator(int windowWidth , int windowHeight);
		// destructor
		~DynamicDistortionCalibrator();

		// stuff for raw distortion aka local offset in x & y direction
		void findRawDistortion();
		void saveRawDistortion();
		void loadRawDistortion();

	private:
		cameraBorder _border;
		int _windowHeight, _windowWidth;
		calibrationImage _vertical, _horizontal;

		int findPixelSize();
		cameraBorder findCameraBorder(int pixelSize);
		void calculateOffset();
		void correctForOffset();
		void createImages(int pixelSize);
    
};