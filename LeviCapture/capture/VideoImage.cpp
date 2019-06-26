#include "VideoImageSaver.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Log.h"

using namespace std;
using namespace capture;

CameraImageSaver::CameraImageSaver(VideoLoader* videoRetriever, ImageSaver* imageSaver) :
	VideoSaver(videoRetriever), _saver(imageSaver) {}

CameraImageSaver::~CameraImageSaver() {
	delete _saver;
}

void CameraImageSaver::beforeSave() {}

void CameraImageSaver::saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo) {
	std::stringstream path;
	path << folderName << "/" << std::to_string(frameNo);
	bool nRet = _saver->saveImage(*frame, path.str());
	if (!nRet) logError("Impossible to save a frame");
	delete frame;
}

void CameraImageSaver::afterSave(std::string folderName) {}