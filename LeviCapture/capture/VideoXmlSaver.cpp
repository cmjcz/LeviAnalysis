
#include "VideoXmlSaver.h"
#include "Camera.h"
#include <fstream>
#include "Bead.h"
#include <sstream>
#include "log.h"

using namespace capture;
using namespace std;

VideoXmlSaver::VideoXmlSaver(VideoLoader* videoRetriever, unsigned long framerate) :
	VideoSaver(videoRetriever), _framerate(framerate), _beadDetector(BeadDetector(100, 60, 0.1)) {}

VideoXmlSaver::~VideoXmlSaver() {}

void VideoXmlSaver::beforeSave() {
	_content << "<MOVEMENT>" << std::endl << "\t<FRAMERATE>" << _framerate << "</FRAMERATE>" << std::endl;
}

void VideoXmlSaver::saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo) {
	Bead* b = _beadDetector.detectBead(frame->toOpenCvMat());
	if (b == nullptr) {
		stringstream ss; ss << "Bead not found on frame " << frameNo;
		logWarning(ss.str());
	}
	else {
		_content << "\t<FRAME n=" << frameNo << ">" << std::endl
			<< "\t\t<X>" << b->getCenterInPx().x << "</X>" << std::endl
			<< "\t\t<Y>" << b->getCenterInPx().y << "</Y>" << std::endl
			<< "\t</FRAME>" << std::endl;
		delete b;
	}
	delete frame;
}

void VideoXmlSaver::afterSave(std::string folderName) {
	_content << "</MOVEMENT>";
	string path = folderName + "\\out.xml";
	ofstream fstream(path);
	fstream << _content.str();
	fstream.close();
}