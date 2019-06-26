/*
	CaptureController.h
	
	This is a controller who is implementing some functions of the Capture Project.
	It is not necessary to use it, and we can do a lot more that just what it is programmed here.

	@author Baptiste Senac
*/

#pragma once
#ifndef CAPTURE_CONTROLLER_H
#define CAPTURE_CONTROLLER_H

#include "Capture.h"

namespace capture {
	class CaptureController {
	public:
		CaptureController();
		~CaptureController();
		bool isUsable();

		void startLiveWindow();
		void toggleLiveWindowPause();
		void stopLiveWindow();

		void takeVideo();
		void takeScreenShot();

		void setCaptureLens(Lens* lens);

		Camera& getCamera();
	private:
		Camera _camera;
		VideoWindow* _camWindow;
		bool _usable;
		Lens* _lens;
	};
}

#endif