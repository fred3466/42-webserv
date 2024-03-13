#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

//int Config::idRef = 0;

Config::Config() :
		kv()
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

std::map<std::string, std::string>* Config::getParamStrStartingWith(std::string paramPrefix)
{
//	TODO un new par ici !
	std::map<std::string, std::string> *ret = new std::map<std::string, std::string>();
	for (std::map<std::string, std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string key = ite->first;
		if (key.find(paramPrefix) == 0)
		{
			std::string val = ite->second;
			(*ret)[key] = val;
		}
	}
	return ret;
}

std::string
Config::getAlias()
{
	return alias;
}

void Config::setAlias(std::string alias)
{
	this->alias = alias;
}

Config::Config(Config &bis)
:
		kv(bis.kv), alias(bis.alias)
{
	if (this != &bis)
		*this = bis;
}

Config& Config::operator =(Config &bis)
{
	this->alias = bis.alias;
	this->kv = bis.kv;
	return bis;
}
