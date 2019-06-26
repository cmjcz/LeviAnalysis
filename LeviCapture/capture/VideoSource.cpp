#include "VideoSource.h"

using namespace capture;

VideoSource::VideoSource(resolution res, Lens* lens) : _res(res), _lens(lens) {}
resolution VideoSource::getResolution() { return _res; }
VideoSource::~VideoSource(){}

Frame * VideoSource::getFrame(unsigned long frameNo) {
	Frame* pFrame = getRawFrame(frameNo);
	if (pFrame && _lens) { pFrame = _lens->apply(pFrame); }
	return pFrame;
}

Frame* VideoSource::getNextFrame() {
	Frame* pFrame = getNextRawFrame();
	if (pFrame && _lens) { pFrame = _lens->apply(pFrame); }
	return pFrame;
}

FramesVideoSource::FramesVideoSource(std::vector<Frame*> frames, resolution res, Lens* lens) :
	VideoSource(res, lens), _frames(frames), _currentIndex(0) {}

FramesVideoSource::~FramesVideoSource() {
	unsigned int size = _frames.size();
	for (unsigned int i = 0; i < size; ++i) {
		delete _frames[i];
	}
}

Frame* FramesVideoSource::getRawFrame(unsigned long frameNo) {
	if (frameNo >= _frames.size()) return nullptr;
	cv::Mat mat = _frames[frameNo]->toOpenCvMat();
	MatFrame* newFrame = new MatFrame(mat.clone(), _res);
	return newFrame;
}

Frame* FramesVideoSource::getNextRawFrame() {
	unsigned int i = _currentIndex;
	if (_currentIndex < _frames.size() - 1) ++_currentIndex;
	else _currentIndex = 0;
	return getRawFrame(i);
}