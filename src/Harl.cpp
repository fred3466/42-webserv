#include "Harl.h"

#include <ctime>
#include <cstring>

#include "config.h"
#include "util/StringUtil.h"
#include <ostream>

Harl::Harl() {
}

Harl::~Harl() {
}
int bufferLen = 2000;

std::time_t Harl::_timeStart = std::time(NULL);

void Harl::trace2(std::string msg, ...) {
	if (HARL_LEVEL >= 5) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(5, "TRACE2", formatedChars);
		delete[] formatedChars;
	}
}
void Harl::trace(std::string msg, ...) {
	if (HARL_LEVEL >= 4) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(4, "TRACE", formatedChars);
		delete[] formatedChars;
	}
}
void Harl::debug(std::string msg, ...) {
	if (HARL_LEVEL >= 3) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(3, "DEBUG", formatedChars);
		delete[] formatedChars;
	}
}

void Harl::info(std::string msg, ...) {
	if (HARL_LEVEL >= 2) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(2, "INFO", formatedChars);
		delete[] formatedChars;
	}
}

void Harl::warning(std::string msg, ...) {
	if (HARL_LEVEL >= 1) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(1, "WARNING", formatedChars);
		delete[] formatedChars;
	}
}

void Harl::error(std::string msg, ...) {
	if (HARL_LEVEL >= 0) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(0, "[!][!][!] ERROR", formatedChars);
		delete[] formatedChars;
	}
}

void Harl::except(std::string msg, ...) {
	if (HARL_LEVEL >= -1) {
		char *formatedChars = new char[bufferLen + 1];
		memset(formatedChars, 0, bufferLen + 1);
		va_list args;
		va_start(args, msg);
		vsnprintf(formatedChars, bufferLen, msg.c_str(), args);
		va_end(args);
		Harl::_write(0, "EXCEPT", formatedChars);
		delete[] formatedChars;
	}
}

void Harl::_write(int level, std::string levelStr, std::string msg) {
	std::time_t now = std::time(NULL);
//	std::ofstream outfile("debug.txt", std::ios_base::app);
	std::string pretab = "";
	for (int i = 0; i < level; i++) {
		pretab += "\t\t";
	}

	std::cout << pretab << ((double) (now - _timeStart)) << "\t" << getpid() << "\t" << levelStr.c_str() << "\t" << msg << std::endl;
}

