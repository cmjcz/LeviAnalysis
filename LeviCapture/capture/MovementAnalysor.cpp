#include "MovementAnalysor.h"
#include "BeadDetector.h"
#include "log.h"
#include <sstream>
#include <cmath>

using namespace capture;
using namespace cv;

MovementAnalysor::MovementAnalysor(BeadDetector& detector, float ignoreDeltaInPx) :
	_beadDetector(detector), _ignoreDeltaInPx(ignoreDeltaInPx), _pBaseBead(nullptr) {
}

MovementAnalysor::MovementAnalysor(cv::Mat const base, BeadDetector& detector, float ignoreDeltaInPx) :
	MovementAnalysor(detector, ignoreDeltaInPx) {
	setBase(base);
}

MovementAnalysor::~MovementAnalysor() {
	delete _pBaseBead;
}

bool MovementAnalysor::hasMoved(Mat const newFrame) const {
	movement_2d_vector shifting_vector = findRelativePositionInPx(newFrame);
	if (shifting_vector.x == -1 || shifting_vector.y == -1) return false;
	double shifting = sqrt(pow(shifting_vector.x, 2) + pow(shifting_vector.y, 2));
	bool moved = shifting >= _ignoreDeltaInPx;
	return moved;
}

movement_2d_vector MovementAnalysor::findRelativePositionInPx(cv::Mat const newFrame) const {
	Bead* newBead = _beadDetector.detectBead(newFrame);
	if (!newBead) {
		return movement_2d_vector{ -1, -1 };
	}
	if (!_pBaseBead) {
		return movement_2d_vector{ -1, -1 };
	}
	movement_2d_vector res= findRelativePositionInPx(*newBead);
	delete newBead;
	return res;
}

movement_2d_vector MovementAnalysor::findRelativePositionInPx(Bead const & bead) const {
	double xShifting = (double(bead.getCenterInPx().x) - _pBaseBead->getCenterInPx().x);
	double yShifting = (double(bead.getCenterInPx().y) - _pBaseBead->getCenterInPx().y);
	return movement_2d_vector{ xShifting, yShifting };
}

movement_2d_vector MovementAnalysor::findRelativePositionInMm(cv::Mat const newFrame) const {
	Bead* newBead = _beadDetector.detectBead(newFrame);
	if (!newBead) {
		return movement_2d_vector{ -1, -1 };
	}
	if (!_pBaseBead) {
		return movement_2d_vector{ -1, -1 };
	}
	movement_2d_vector res = findRelativePositionInMm(*newBead);
	delete newBead;
	return res;
}

movement_2d_vector MovementAnalysor::findRelativePositionInMm(Bead const& bead) const {
	double xShifting = (double(bead.getCenterInMm().x) - _pBaseBead->getCenterInMm().x);
	double yShifting = (double(bead.getCenterInMm().y) - _pBaseBead->getCenterInMm().y);
	return movement_2d_vector{ xShifting, yShifting };
}

void MovementAnalysor::setBase(Mat const newFrame) {
	_pBaseBead = _beadDetector.detectBead(newFrame);
}

bool MovementAnalysor::isBaseSet() {
	return _pBaseBead;
}

