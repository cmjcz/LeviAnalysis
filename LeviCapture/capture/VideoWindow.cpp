#include "VideoWindow.h"
#include "log.h"
#include "opencv2/highgui.hpp"
#include <sstream>
#include <thread>
#include <chrono>
using namespace capture;

VideoWindow::VideoWindow(VideoSource& videoSource) :
    _videoSource(videoSource), _paused(false), _shouldClose(false) {
	
	// Get videoSource's resolution
	resolution res = videoSource.getResolution();
	_width = res.width;
	_heigh = res.heigh;

}

VideoWindow::~VideoWindow() {
}


void VideoWindow::togglePaused() {
	_paused = !_paused;
}

bool VideoWindow::isPaused() {
	return _paused;
}

void VideoWindow::display() {
	while(!_shouldClose)
	{
		if (_paused) continue;
		
		// Get the image
		Frame* pFrame = _videoSource.getNextFrame();
		if (nullptr == pFrame) continue;

		// Show the image
		cv::imshow(_title, pFrame->toOpenCvMat());
		cv::waitKey(1);


		// Free memory
		delete pFrame;
	}
}

void VideoWindow::shouldClose() {
	_shouldClose = true;
}