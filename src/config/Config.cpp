#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

// int Config::idRef = 0;

Config::Config() : kv(), alias("")
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
		std::string res = kv.at(param);
		if (!res.empty())
		{
			int resInt = StringUtil().intFromStr(res);
			return resInt;
		}
	}
	catch (const std::out_of_range &oor)
	{
		//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return intDefault;
}

int Config::getParamInt(const std::string &key)
{
	int value;
	if (!tryGetValue(key, value))
	{
		throw std::runtime_error("Required configuration parameter missing: " + key);
	}
	return value;
}

void Config::addParam(std::string param, std::string value)
{
	kv[param] = value;
}

std::string Config::getParamStr(std::string param, std::string stringDefault)
{
	try
	{
		std::string res = kv.at(param);
		if (!res.empty())
		{
			return res;
		}
	}
	catch (const std::out_of_range &oor)
	{
		//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return stringDefault;
}

std::map<std::string, std::string> *Config::getParamStrStartingWith(std::string paramPrefix)
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
	: alias(bis.alias)
{
	for (std::map<std::string, std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++)
	{
		std::string key = ite->first;
		std::string val = ite->second;
		kv[key] = val;
	}
	if (this != &bis)
		*this = bis;
}

Config &Config::operator=(Config &bis)
{
	this->alias = bis.alias;

	for (std::map<std::string, std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++)
	{
		std::string key = ite->first;
		std::string val = ite->second;
		kv[key] = val;
	}
	//	this->kv = bis.kv;
	return *this;
}

Config *Config::clone()
{
	Config *ret = new Config();
	ret->alias = alias;

	for (std::map<std::string, std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string key = ite->first;
		std::string val = ite->second;
		ret->kv[key] = val;
	}
	//	this->kv = bis.kv;
	return ret;
}

bool Config::tryGetValue(const std::string &key, int &value)
{
	std::map<std::string, std::string>::iterator it = kv.find(key);
	if (it != kv.end())
	{
		std::istringstream iss(it->second);
		if (!(iss >> value))
		{
			return false;
		}
		return true;
	}
	return false;
}

int Config::getRouteSpecificMaxBodySize(const std::string &route, int defaultSize)
{
	// Directly check for the specific route
	if (route == "/post_body")
	{
		return 100; // Set specific limit for this route as required
	}

	// Otherwise, use the general settings from the configuration
	std::string key = "maxBodySize:" + route; // Prefix to distinguish this type of config value
	std::map<std::string, std::string>::iterator it = kv.find(key);
	if (it != kv.end())
	{
		return std::atoi(it->second.c_str()); // Convert the string to int safely for C++98
	}
	return defaultSize; // Return the default size if no specific configuration is found
}

// Config::Config(Config &bis) : kv(bis.kv), alias(bis.alias) {}

// Config &Config::operator=(Config &bis)
// {
// 	if (this != &bis)
// 	{
// 		alias = bis.alias;
// 		kv = bis.kv;
// 	}
// 	return *this;
// }
