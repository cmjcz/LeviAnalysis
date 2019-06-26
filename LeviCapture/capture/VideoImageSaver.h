#pragma once
/**
	VideoImageSaver.h

	VideoImageSaver save a given video inside a set of image, using a given ImageSaver

	@author Baptiste Senac
*/

#pragma once
#ifndef CAMERA_IMAGE_SAVER_H
#define CAMERA_IMAGE_SAVER_H

#include "Camera.h"
#include "VideoSaver.h"

namespace capture {

	class CameraImageSaver : public VideoSaver {
	public:
		CameraImageSaver(VideoLoader* videoRetriever, ImageSaver* imageSaver);
		virtual ~CameraImageSaver();
	protected:
		ImageSaver* _saver;

		virtual void beforeSave();
		virtual void saveOneFrame(std::string folderName, Frame* frame, unsigned long frameNo);
		virtual void afterSave(std::string folderName);
	};
}

#endif