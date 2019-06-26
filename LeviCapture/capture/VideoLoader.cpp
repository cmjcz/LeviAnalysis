#include "VideoLoader.h"
#include "log.h"
#include <sstream>

using namespace capture;

VideoLoader::VideoLoader(VideoSource& videoSource) : _videoSource(videoSource) {}

void VideoLoader::retrieveImages(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const {
	unsigned short int errNo;
	unsigned long firstFrame = findFirstFrame(startFrame, endFrame);
	pBuffer.setWrittingFinished(false);
	unsigned long lastFrame = saveUntilLastFrame(pBuffer, firstFrame, endFrame);
	pBuffer.setWrittingFinished(true);
}


SimpleVideoRetriever::SimpleVideoRetriever(VideoSource& videoSource) : VideoLoader(videoSource) {}


unsigned long SimpleVideoRetriever::findFirstFrame(unsigned long startFrame, unsigned long endFrame)  const{
	return startFrame;
}
unsigned long SimpleVideoRetriever::saveUntilLastFrame(FrameBuffer& pBuffer, unsigned long startFrame, unsigned long endFrame) const {
	bool found = false;
	unsigned short int errNo;
	resolution res = _videoSource.getResolution();
	double max = 0;
	float delta_in_mm = 5;
	for(unsigned long frameNo = startFrame; frameNo < endFrame; ++frameNo) {
		Frame* frame = _videoSource.getFrame(frameNo);
		if (nullptr == frame) {
			return -1;
		}
		pBuffer.push(frame, frameNo);
	}
	std::stringstream ss;
	logInfo(ss.str());
	return endFrame;
}