/**
	Log.h

	Standart IO for the capture related functions.

	@author Baptiste Senac
*/

#pragma once

#ifndef LOG_INFO_H
#define LOG_INFO_H

#include <iostream>
#include <string>

namespace capture {

	inline static void log(const char* header, const std::string msg, std::ostream& out) {
		out << header << " " << msg << std::endl;
	}

	inline void logInfo(const std::string msg) {
		log("[CAPTURE INFO]", msg, std::cout);
	}

	inline void logError(const std::string msg) {
		log("[CAPTURE ERROR]", msg, std::cerr);
	}

	inline void logWarning(const std::string msg) {
		log("[CAPTURE WARNING]", msg, std::cerr);
	}

	inline void waitForAction(const std::string msg) {
		log("[CAPTURE ACTION]", msg + " Press enter to continue", std::cout);
		std::cin.ignore();
	}
}

#endif
