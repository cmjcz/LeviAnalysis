/**
	ImageSaver.h
	
	The ImageSaver is an object who tell how to save a Frame into a file.
	Saver is an abstract class, it need to be specialized to implement the saving.
	Naming convention: [ImageType]Saver

	@author Baptiste Senac
*/

#pragma once

#ifndef SAVER_H
#define SAVER_H

#include "Camera.h"
#include "opencv2/core/core.hpp"
#include "Frame.h"
#include <string>

namespace capture {
	class Camera;
	class ImageSaver {
	public:
		ImageSaver(resolution res);
		ImageSaver();
		// Save an image into the specified path
		virtual bool saveImage(Frame& frame, std::string path) const = 0;
	protected:
		resolution _res;
	};

	class JpegSaver : public ImageSaver {
	public:
		JpegSaver(resolution res);
		virtual bool saveImage(Frame& frame, std::string path) const;
	};
}

#endif
