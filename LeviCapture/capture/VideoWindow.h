/**
	VideoWindow.h

	The camera window is an opencv window who show the current Frame of a VideoSource.

	@author Baptiste Senac
*/

#pragma once

#ifndef CAMERA_SCREEN_H
#define CAMERA_SCREEN_H

#include <string>
#include "VideoSource.h"
#include "Frame.h"
namespace capture {
	class VideoWindow {
	public:
		VideoWindow(VideoSource& videoSource);
		~VideoWindow();
		void display();
		void shouldClose();
		void togglePaused();
		bool isPaused();
	private:
		const char* _title = "Camera";

		unsigned int _width, _heigh;
		VideoSource& _videoSource;

		bool _paused;
		bool _shouldClose;

		int _nbFrames = 0;
		double _lastTime = 0;
	};
}

#endif // !CAMERA_SCREEN_H

