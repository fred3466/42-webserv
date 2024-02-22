#include "Harl.h"
#include <ctime>
Harl::Harl()
{
}

Harl::~Harl()
{
}

void Harl::debug(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 3)
	{
		va_start(args, msg);
		int nbCharsWritten = vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("DEBUG", formatedChars);
	}
}

void Harl::info(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 2)
	{
		va_start(args, msg);
		int nbCharsWritten = vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("INFO", formatedChars);
	}
}

void Harl::warning(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 1)
	{
		va_start(args, msg);
		int nbCharsWritten = vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("WARNING", formatedChars);
	}
}

void Harl::error(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 0)
	{
		va_start(args, msg);
		int nbCharsWritten = vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("ERROR", formatedChars);
	}
}

void Harl::_write(std::string level, std::string msg)
{
	char buf[100];
	time_t rawtime;
//	va_list args;
	std::string out;
//	int nbCharsWritten;

	time(&rawtime);

//	va_start(args, msg);
//	nbCharsWritten = vsnprintf(formatedChars, 2000, msg.c_str(), args);
//	va_end(args);

	tm *_time = localtime(&rawtime);
	strftime(buf, sizeof(buf), ((std::string) "%T%S.000").c_str(), _time);
	std::cout << level << "\t" << msg << std::endl;
}

