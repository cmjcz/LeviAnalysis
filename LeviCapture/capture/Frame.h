/*
	Frame.h

	A Frame is an astract object who is representing a image.
	You must inherit from Frame to create a frame from a custom datastructure

	@author Baptiste Senac
*/

#pragma once
#ifndef TYPES_H
#define TYPES_H

#include "opencv2/core/core.hpp"

namespace capture {
	class Frame;
	class MatFrame;
	class CameraFrame;

	typedef unsigned char* frame;

	struct resolution {
		unsigned int width;
		unsigned int heigh;
	};

	class Frame {
	public:
		inline Frame(resolution res) : _res(res) {}
		inline virtual ~Frame() {}
		virtual unsigned char at(int line, int column) const = 0;
		virtual cv::Mat toOpenCvMat() const = 0;
	protected:
		resolution _res;
	};

	class MatFrame : public Frame {
	public:
		inline MatFrame(cv::Mat mat, resolution res) : Frame(res), _mat(mat.clone()) {}
		inline virtual ~MatFrame() {}
		inline virtual unsigned char at(int line, int column) const { return _mat.at<unsigned char>(column, line); }
		virtual cv::Mat toOpenCvMat() const { return _mat.clone(); }
	private:
		cv::Mat _mat;
	};

	class CameraFrame : public Frame {
	public:
		inline CameraFrame(frame frame, resolution res) : Frame(res), _frame(frame) {}
		inline virtual ~CameraFrame() { delete[] _frame; }
		inline virtual unsigned char at(int line, int column) const { 
			return _frame[column + line * _res.width] ; }
		virtual cv::Mat toOpenCvMat() const {
			cv::Mat mat = cv::Mat(_res.heigh, _res.width, CV_8UC1, _frame).clone();
			return mat;
		}
	private:
		frame _frame;
	};
}

//void applyHomography(frame frame);
#endif
