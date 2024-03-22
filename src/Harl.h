#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>

class Harl
{
private:
	void _write(std::string level, std::string msg);
	public:
	Harl();
	~Harl();
	void trace(std::string msg, ...);
	void debug(std::string msg, ...);
	void info(std::string msg, ...);
	void warning(std::string msg, ...);
	void error(std::string msg, ...);
	void except(std::string msg, ...);

};
