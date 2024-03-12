#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

//int Config::idRef = 0;

Config::Config() :
		kv(), alias("")
{
//	if (idRef == NULL)
//		idRef = 0;
//	alias = idRef++;
}

Config::~Config()
{
}

int Config::getParamInt(std::string param, int intDefault)
{
	try
	{
		std::string res = kv[param];
		if (!res.empty())
		{
			std::stringstream ss(res);
			int resInt;
			ss >> resInt;
			return resInt;
		}
	}
	catch (const std::out_of_range &oor)
	{
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return intDefault;
}

void Config::addParam(std::string param, std::string value)
{
	kv[param] = value;
}

std::string Config::getParamStr(std::string param, std::string stringDefault)
{
	try
	{
		std::string res = kv[param];
		if (!res.empty())
		{
			return res;
		}
	}
	catch (const std::out_of_range &oor)
	{
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return stringDefault;
}

//void Config::setId(std::string id)
//{
//	this->id = id;
//}

std::string Config::getAlias()
{
	return getParamStr("alias", "");
}

Config::Config(Config &bis) :
		kv(bis.kv)
{
	this->kv = bis.kv;
	if (this != &bis)
		*this = bis;
}

Config& Config::operator =(Config &bis)
{
	this->kv = bis.kv;
	return bis;
}
