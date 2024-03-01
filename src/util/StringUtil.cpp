#include "StringUtil.h"

StringUtil::StringUtil()
{
}

StringUtil::~StringUtil()
{
}

std::string StringUtil::strUpper(std::string s)
{
	std::string ret = s;
	for (std::string::iterator ite = ret.begin(); ite != ret.end(); ite++)
	{
		*ite = toupper(*ite);
	}
	return ret;
}

std::string StringUtil::fromListToString(std::list<std::string> l)
{
	std::string ret;
	for (std::list<std::string>::iterator ite = l.begin(); ite != l.end();
			ite++)
	{
		ret += *ite;
	}
	return ret;
}
