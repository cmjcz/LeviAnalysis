/**
	VideoSaver.h

	Abstract.
	A VideoSaver save on the disk the Frames from a FrameBuffer, in the wanted format.

	@author Baptiste Senac
*/

#pragma once
#ifndef CAMERA_SAVER_H
#define CAMERA_SAVER_H

#include "VideoLoader.h"
#include <string.h>
#include "FrameBuffer.h"

namespace capture {
	class VideoSaver {
	public:
		VideoSaver(VideoLoader* videoRetriever);
		~VideoSaver();
		bool saveVideo(std::string path, size_t startFrame, size_t endFrame);
	protected:
		FrameBuffer _buffer;
		VideoLoader* _videoRetriever;

		virtual void beforeSave() = 0;
		virtual void saveOneFrame(std::string folderName, Frame * frame, unsigned long frameNo) = 0;
		virtual void afterSave(std::string folderName) = 0;
	};
}

#endif