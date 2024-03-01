#pragma once
#include <cstring>
#include <string>
#include <list>

class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	std::string strUpper(std::string s);
	std::string fromListToString(std::list<std::string> l);
};

