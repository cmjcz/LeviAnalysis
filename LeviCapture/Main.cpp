#include "capture/Capture.h"
#include <filesystem>
#include <ctype.h>
namespace fs = std::experimental::filesystem;

#define EXPECTED_ARG_NB 10
#define NB_INT_ARGS 4
#define separator "/"

using namespace capture;
using namespace std;

int main(int argc, char* argv[]) {
	// Parameters analysis
	if (argc < EXPECTED_ARG_NB) {
		logError(to_string(EXPECTED_ARG_NB - 1) + " argmuments needed : [data path] [frames directory name] [back frame name] [mesure frame name] [output folder] [background width in mm] [background height in mm] [bead diameter in mm] [framerate]");
		return 1;
	}
	string path = argv[1], out_folder = argv[2], back_frame = argv[3], mesure_frame = argv[4], out_path = argv[5];
	unsigned int values[NB_INT_ARGS];
	for (unsigned int i = EXPECTED_ARG_NB - NB_INT_ARGS; i < EXPECTED_ARG_NB; ++i) {
		string arg = string(argv[i]);
		if (!arg.empty() && arg.find_first_not_of("0123456789") == std::string::npos) {
			values[i - EXPECTED_ARG_NB + NB_INT_ARGS] = stoi(arg);
		}
		else {
			logError("Invalid argument. You must provide an unsigned integer at position " + to_string(i));
			return 1;
		}
	}
	float back_width = float(values[0]),
		back_height = float(values[1]), bead_diameter = float(values[2]);
	unsigned long framerate = values[3];
	bool isCorrectFolder = false;
	if (fs::exists(path)) isCorrectFolder = fs::is_directory(path);
	if (!isCorrectFolder) {
		logError("The provided folder doesn't exist !");
		return 1;
	}
	if (!fs::exists(path + separator + back_frame) || !fs::exists(path + separator + mesure_frame)) {
		logError("File not found. Please check the name of the back frame or the mesure frame.");
		return 1;
	}
	if (!fs::exists(out_path)) {
		logError("File not found. Please check the output directory");
		return 1;
	}

	// Program

	logInfo("Initalizing");
	cv::Mat backMat = cv::imread(path + separator + back_frame, cv::ImreadModes::IMREAD_GRAYSCALE);
	cv::Mat mesureMat = cv::imread(path + separator + mesure_frame, cv::ImreadModes::IMREAD_GRAYSCALE);
	resolution res = {unsigned int(backMat.cols), unsigned int(backMat.rows)};
	Frame* backFrame = new MatFrame(backMat, res);
	Frame* mesureFrame = new MatFrame(mesureMat, res);
	HomographyLens* pLens = new HomographyLens(backFrame, mesureFrame);
	FileVideoSource videoSource(path + separator + out_folder, res, pLens);
	BeadDetector detector(back_width, back_height, bead_diameter);
	Frame* base = videoSource.getFrame(0);
	cv::Mat baseMat = base->toOpenCvMat();
	MovementAnalysor* analysor = new MovementAnalysor(baseMat, detector);
	delete base;
	MovementVideoLoader* loader = new MovementVideoLoader(videoSource, analysor);
	VideoCsvSaver saver(loader, framerate);
	std::size_t size = videoSource.size();
	if (0 == size) {
		logError("Out folder empty !");
		return 1;
	}
	logInfo("Starting to export");
	saver.saveVideo("D:", 0, size);
	logInfo("Done !");
	return 0;
}