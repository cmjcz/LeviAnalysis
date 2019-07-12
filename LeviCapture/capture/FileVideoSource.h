/*
	FileVideoSource.h

	A FileVideoSource represent a folder where we can found each frame individually

	@author Baptiste Senac
*/

#pragma once

#ifndef FILE_VIDEO_SOURCE_H
#define FILE_VIDEO_SOURCE_H

#include "VideoSource.h"
#include <regex>
#include <vector>
#include <string>

namespace capture {
	class FileVideoSource : public VideoSource {
	public:
		FileVideoSource(std::string folderName, resolution res, Lens* lens = nullptr);
		virtual ~FileVideoSource();
		virtual std::size_t size();
	private:
		std::string _folderName;
		std::vector<std::string> _files;
		unsigned long _currentIndex;

	protected:
		virtual Frame* getRawFrame(unsigned long frameNo);
		virtual Frame* getNextRawFrame();
	};
}

#endif