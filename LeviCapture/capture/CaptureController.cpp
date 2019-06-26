#include "CaptureController.h"
#include <ctime>
#include <iomanip>
#define BEAD_SIZE 5
using namespace capture;

CaptureController::CaptureController() : _usable(false), _camWindow(nullptr), _lens(nullptr) {
	bool nRet = _camera.searchCamera();
	if (!nRet) return;
	nRet = _camera.openCamera();
	_usable = nRet;
}

CaptureController::~CaptureController() {
	delete _lens;
	delete _camWindow;
}

bool CaptureController::isUsable() {
	return _usable;
}

void CaptureController::startLiveWindow() {
	resolution res = _camera.getCurrentResolution();
	CameraLiveVideoSource cameraSource(_camera, res, _lens);
	_camWindow = new VideoWindow(cameraSource);
	_camWindow->display();
}

void CaptureController::toggleLiveWindowPause() {
	_camWindow->togglePaused();
}

void CaptureController::stopLiveWindow() {
	_camWindow->shouldClose();
	delete _camWindow;
}

void CaptureController::takeScreenShot() {
	resolution res = _camera.getCurrentResolution();
	CameraLiveVideoSource videoSource(_camera, res);
	JpegSaver saver(res);
	Frame* frame = videoSource.getNextFrame();
	std::time_t const now = std::time(0);
	struct tm time;
	localtime_s(&time, &now);
	std::stringstream path;
	path << "D:\\camshot-" << std::put_time(&time, "%y-%b-%d-%H-%M");
	saver.saveImage(*frame, path.str());
}

void CaptureController::takeVideo() {
	bool nRet;
	logInfo("starting to record");
	nRet = _camera.startRecording();
	if (nRet) logInfo("record finished");
	else {
		logError("impossible to record. Abort.");
		return;
	}

	logInfo("starting to save the video");
	if (!_camWindow->isPaused()) _camWindow->togglePaused();
	unsigned long fps = _camera.getFrameRate();
	resolution res = _camera.getCurrentResolution();
	unsigned long startFrame = 0;
	unsigned long endFrame = fps;
	BeadDetector beadDetector(140, 70, BEAD_SIZE);
	MovementAnalysor* pMovementDetector;
	unsigned int i = 0;
	do {
		frame buffer = _camera.retrieveMemImage(i);
		Frame* frame = new CameraFrame(buffer, res);
		frame = _lens->apply(frame);
		pMovementDetector = new MovementAnalysor(frame->toOpenCvMat(), beadDetector);
		delete frame;
		++i;
	} while (!pMovementDetector->isBaseSet());
	CameraMemVideoSource videoSource(_camera, res, _lens);
	//VideoXmlSaver saver(new MovementVideoLoader(videoSource, pMovementDetector), fps);
	CameraImageSaver image_saver(new MovementVideoLoader(videoSource, pMovementDetector), new JpegSaver(res));
	VideoCsvSaver csv_saver(new MovementVideoLoader(videoSource, pMovementDetector), fps);
	//CameraImageSaver saver(new SimpleVideoRetriever(videoSource), new JpegSaver(res));
	nRet = image_saver.saveVideo(startFrame, endFrame);
	nRet = nRet && csv_saver.saveVideo(startFrame, endFrame);
	if (nRet) logInfo("saved !");
	else {
		unsigned long status = _camera.getStatus();
		std::stringstream ss;
		ss << "impossible to save ! Current status : " << status << std::endl;
		logError(ss.str());
	}
	if (_camWindow->isPaused()) _camWindow->togglePaused();
}

void CaptureController::setCaptureLens(Lens* lens) {
	if (_lens != nullptr) delete _lens;
	_lens = lens;
}

Camera& CaptureController::getCamera() {
	return _camera;
}