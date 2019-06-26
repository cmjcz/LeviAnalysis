/**
	VideoXmlSaver.h

	VideoXmlSaver use an video to detect the movement of a bead, and save the informations inside a XML file.

	@author Baptiste Senac
*/


#pragma once
#ifndef CAMERA_XML_SAVER_H
#define CAMERA_XML_SAVER_H

#include "VideoSaver.h"
#include "FrameBuffer.h"
#include "BeadDetector.h"

namespace capture {
	class VideoXmlSaver : public VideoSaver {
	public:
		VideoXmlSaver(VideoLoader* videoRetriever, unsigned long framerate);
		virtual ~VideoXmlSaver();
	protected:
		virtual void beforeSave();
		virtual void saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo);
		virtual void afterSave(std::string folderName);
	private:
		BeadDetector _beadDetector;
		unsigned long _framerate;
		std::stringstream _content;
	};
}

#endif#pragma once
