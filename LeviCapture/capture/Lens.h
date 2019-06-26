/*
	Lens.h

	A Lens is an object who is applying transformations on a Frame.
	It can be followed of another Lens, and the transformation of the next Lens
	will be applyed after the transformation of this Lens.

	@author Baptiste Senac
*/

#pragma once

#ifndef  LENS_H
#define LENS_H
#include "Frame.h"

namespace capture {
	class Lens {
	public:
		Lens(Lens* nextLens = nullptr) : _nextLens(nextLens) {}
		inline Frame* apply(Frame const* frame) const {
			Frame* pFrame = correctFrame(frame);
			if (_nextLens != nullptr) pFrame = _nextLens->apply(pFrame);
			return pFrame;
		}
	protected:
		virtual Frame* correctFrame(Frame const* frame) const = 0;
	private:
		Lens* _nextLens;
	};
}

#endif // ! LENS_H

