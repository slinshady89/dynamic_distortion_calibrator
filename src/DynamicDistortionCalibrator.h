#include "cameraBorder.h"
#include "PixelSizeDetector.h"
#include "CameraBorderDetector.h"



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
		cameraBorder _area;
		int _windowHeight, _windowWidth;

		int findPixelSize();
		cameraBorder findCameraBorder(int pixelSize);
		void calculateOffset();
		void correctForOffset();

    
};