#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>

//"debug"=3, "info"=2, "warning"=1, "error"=0
#define HARL_LEVEL 3

class Harl {
private:
	void _write(std::string level, std::string msg);
public:
	Harl();
	~Harl();
	void debug(std::string msg, ...);
	void info(std::string msg, ...);
	void warning(std::string msg, ...);
	void error(std::string msg, ...);

};
