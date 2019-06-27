#include "MovementVideoRetriever.h"
#include "log.h"

using namespace capture;

MovementVideoLoader::MovementVideoLoader(VideoSource& videoSource, MovementAnalysor* movementDetector) : 
	VideoLoader(videoSource), _movementDetector(movementDetector) {}
MovementVideoLoader::~MovementVideoLoader() {
	delete _movementDetector;
}

unsigned long MovementVideoLoader::findFirstFrame(unsigned long startFrame, unsigned long endFrame) const {
	if (endFrame - startFrame < 2) 
		return startFrame;
	unsigned long currentFrame = (endFrame + startFrame) / 2;
	unsigned short int errNo;
	resolution res = _videoSource.getResolution();
	Frame* frame = _videoSource.getFrame(currentFrame);
	if (nullptr == frame) {
		return -1;
	}
	cv::Mat ocvframe = frame->toOpenCvMat();
	bool moved = _movementDetector->hasMoved(ocvframe);
	if (moved) {
		return findFirstFrame(startFrame, currentFrame);
	}
	else {
		return findFirstFrame(currentFrame, endFrame);
	}
}

unsigned long MovementVideoLoader::saveUntilLastFrame(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const {
	unsigned long frameNo = startFrame, maxFrameNo = frameNo;
	bool found = false;
	unsigned short int errNo;
	resolution res = _videoSource.getResolution();
	double max = 0; 
	unsigned int delta_in_px = 10;
	while (!found && frameNo < endFrame) {
		Frame* frame = _videoSource.getFrame(frameNo);
		if (nullptr == frame) {
			return -1;
		}
		cv::Mat ocvframe = frame->toOpenCvMat();

		movement_2d_vector relativePosition = _movementDetector->findRelativePositionInPx(ocvframe);
		if (relativePosition.x == -1 || relativePosition.y == -1) continue; // throw the frame if the bead is not detected
		double shifting = abs(relativePosition.x);
		if (shifting > max) {
			max = shifting;
			maxFrameNo = frameNo;
		}
		else if (shifting < max - delta_in_px) {
			found = true; 
			// If we are too far behing the maximum, 
			// in our simple movemement this is because you are going behind. So we found the end of the movement
		}
		if(!found) pBuffer.push(frame, frameNo);
		++frameNo;
	}
	return maxFrameNo;
}