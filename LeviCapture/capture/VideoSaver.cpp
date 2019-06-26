#include "VideoSaver.h"

#include "log.h"
#include <thread>
#include <chrono>
#include <ctime>    
#include <iomanip>
#include <windows.h>
#include "opencv2/core/core.hpp"
#include <sstream>
#include "MovementAnalysor.h"

using namespace capture;
using namespace std;

VideoSaver::VideoSaver(VideoLoader* videoRetriever) : _videoRetriever(videoRetriever){}
VideoSaver::~VideoSaver() {}
bool VideoSaver::saveVideo(unsigned long startFrame, unsigned long endFrame) {
	thread retrieveImagesThread(&VideoLoader::retrieveImages, _videoRetriever, std::ref(_buffer), startFrame, endFrame);

	std::time_t const now = std::time(0);
	struct tm time;
	localtime_s(&time, &now);
	std::stringstream folderNameSS; 
	folderNameSS << "D:\\out-" << std::put_time(&time, "%y-%b-%d-%H-%M");
	std::string folderName = folderNameSS.str();
	if (!CreateDirectory(folderName.c_str(), NULL)) return false;
	unsigned int pause = 10;
	unsigned long frameNo;
	beforeSave();
	while (!(_buffer.isWrittingFinished() && _buffer.isEmpty())) {
		if (_buffer.isEmpty()) {
			std::this_thread::sleep_for(std::chrono::seconds(pause));
			continue;
		}
		IndexedFrame indexedFrame = _buffer.pop();
		Frame* frame = indexedFrame.frame;
		frameNo = indexedFrame.index;
		saveOneFrame(folderName, frame, frameNo);
	}
	//save(folderNameSS.str(), pause);
	retrieveImagesThread.join();
	afterSave(folderName);
	return true;
}

