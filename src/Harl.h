#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

class Harl {
private:
	void _write(int level, std::string levelStr, std::string msg);
	public:
	Harl();
	~Harl();
	void trace2(std::string msg, ...);
	void trace(std::string msg, ...);
	void debug(std::string msg, ...);
	void info(std::string msg, ...);
	void warning(std::string msg, ...);
	void error(std::string msg, ...);
	void except(std::string msg, ...);

	static std::time_t _timeStart;
};
