#include "Harl.h"

#include <ctime>
#include <cstring>

#include "config.h"

Harl::Harl()
{
}

Harl::~Harl()
{
}

void Harl::trace2(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 5)
	{
		va_start(args, msg);
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("TRACE2", formatedChars);
	}
}
void Harl::trace(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= 4)
	{
		va_start(args, msg);
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("TRACE", formatedChars);
	}
}
void Harl::debug(std::string msg, ...)
{
	char formatedChars[2000];
	memset(formatedChars, 0, 2000);
	va_list args;
	if (HARL_LEVEL >= 3)
	{
		va_start(args, msg);
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
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
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
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
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
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
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("ERROR", formatedChars);
	}
}
void Harl::except(std::string msg, ...)
{
	char formatedChars[2000];
	va_list args;
	if (HARL_LEVEL >= -1)
	{
		va_start(args, msg);
		vsnprintf(formatedChars, 2000, msg.c_str(), args);
		va_end(args);
		Harl::_write("EXCEPT.", formatedChars);
	}
}

void Harl::_write(std::string level, std::string msg)
{
	std::cout << level << "\t" << msg << std::endl;
}

