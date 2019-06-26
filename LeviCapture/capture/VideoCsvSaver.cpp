#include "VideoCsvSaver.h"
#include <sstream>
#include <fstream>

using namespace capture;

VideoCsvSaver::VideoCsvSaver(VideoLoader* videoRetriever, unsigned long framerate, unsigned int widthInMM, unsigned int heightInMM, unsigned int beadRadiusInMM)
: _framerate(framerate), VideoSaver(videoRetriever), _beadDetector(BeadDetector(widthInMM, heightInMM, beadRadiusInMM)), _analysor(MovementAnalysor(_beadDetector, 10)) {}

VideoCsvSaver::~VideoCsvSaver() {}

void VideoCsvSaver::beforeSave() {
	_content << "Time (ms); X (px); Y (px); X (mm); Y (mm); Vx (mm/ms); Vy (mm/ms); Ax (mm/ms²); Ay (mm/ms²)" << std::endl;
}

void VideoCsvSaver::saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo) {
	cv::Mat frameMat = frame->toOpenCvMat();
	delete frame;
	float timeInMs = 0, Vx = 0, Vy = 0, Ax = 0, Ay = 0, dX, dY, dT, dVx, dVy;
	if (isFirstFrame) {
		firstFrame = frameMat.clone();
		_analysor.setBase(firstFrame);
		firstFrameNo = frameNo;
	}
	Bead* b = _beadDetector.detectBead(frameMat);
	if (b) {
		timeInMs = (float(frameNo) - firstFrameNo) / _framerate * 1000;
		mov = _analysor.findRelativePositionInMm(*b);
		if (isFirstFrame) previousMov = mov;

		dX = mov.x - previousMov.x;
		dY = mov.y - previousMov.y;
		dT = (timeInMs - previousTimeInMs);
		if (dT > 0) {
			Vx = dX / dT;
			Vy = dY / dT;
			dVx = Vx - previousVx;
			dVy = Vy - previousVy;
			Ax = dVx / dT;
			Ay = dVy / dT;
		}
		else {
			Vx = 0;
			Vy = 0;
			Ax = 0;
			Ay = 0;
		}

		previousVx = Vx;
		previousVy = Vy;
		previousTimeInMs = timeInMs;
		previousMov = mov;

		_content << timeInMs << ";" << b->getCenterInPx().x << ";" << b->getCenterInPx().y << ";"
			<< b->getCenterInMm().x << ";" << b->getCenterInMm().y << ";"
			<< Vx << ";" << Vy << ";"
			<< Ax << ";" << Ay << std::endl;
		if (isFirstFrame) isFirstFrame = false;
		delete b;
	}
}

void VideoCsvSaver::afterSave(std::string folderName) {
	std::string path = folderName + "\\out.csv";
	std::ofstream fstream(path);
	fstream << _content.str();
	fstream.close();
}