#include "cameraArea.h"
#include "PixelSizeDetector.h"
#include "CameraAreaDetector.h"

class DynamicDistortionCalibrator {
	public:
		// constructor
		DynamicDistortionCalibrator(int windowHeight, int windowWidth);
		// destructor
		~DynamicDistortionCalibrator();

		// stuff for raw distortion aka local offset in x & y direction
		void findRawDistortion();
		void saveRawDistortion();
		void loadRawDistortion();
		

	private:
		cameraArea _area;
		int _windowHeight, _windowWidth;

		int findPixelSize();
		cameraArea findCameraArea(int pixelSize);
		void calculateOffset();
		void correctForOffset();
};