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

bool Config::_deleteEntryForKey(std::string key)
{
	std::string ret = "";
	StringUtil su = StringUtil();
	std::string keyUpper = su.strUpperCase(key);

	for (std::vector<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string rawEntry = *ite;
		std::vector<std::string> toks = su.tokenize(rawEntry, '|');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		name = su.strUpperCase(su.trim(name));
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		val = su.trim(val);
		if (name == keyUpper)
		{
			kv.erase(ite);
			return true;
		}
	}
	return false;
}

std::string Config::_getValueForKey(std::string key)
{
	std::string ret = "";
	StringUtil su = StringUtil();
	std::string keyUpper = su.strUpperCase(key);

	for (std::vector<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string rawEntry = *ite;
		std::vector<std::string> toks = su.tokenize(rawEntry, '|');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		name = su.strUpperCase(su.trim(name));
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		val = su.trim(val);
		if (name == keyUpper)
			return val;
	}
	return ret;
}

int Config::getParamInt(std::string param, int intDefault)
{
	try
	{
//		std::string res = kv.at(param);
		std::string res = _getValueForKey(param);

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

void Config::addOrReplaceParam(std::string param, std::string val)
{
	StringUtil su = StringUtil();

	_deleteEntryForKey(param);

	kv.push_back(param + "|" + val);
}

void Config::addParam(std::string param, std::string value)
{
	kv.push_back(param + "|" + value);
}

std::string Config::getParamStr(std::string param, std::string stringDefault)
{
	try
	{
//		std::string res = kv.at(param);
		std::string res = _getValueForKey(param);
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

std::vector<std::string>* Config::getParamStrStartingWith(std::string paramPrefix)
{
	StringUtil su = StringUtil();
	//	TODO un new par ici !
	std::vector<std::string> *ret = new std::vector<std::string>();

	for (std::vector<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string rawEntry = *ite;

		std::vector<std::string> toks = su.tokenize(rawEntry, '|');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		if (name.find(paramPrefix) == 0)
		{
			ret->push_back(rawEntry);
		}
	}

	return ret;
}

std::string Config::getAlias()
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
	for (std::vector<std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++)
	{
		std::string bisEntry = std::string(*ite);
		kv.push_back(bisEntry);
	}
	if (this != &bis)
		*this = bis;
}

Config& Config::operator=(Config &bis)
{
	this->alias = bis.alias;

	for (std::vector<std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++)
	{
		std::string bisEntry = std::string(*ite);
		kv.push_back(bisEntry);
	}
	return *this;
}

Config* Config::clone()
{
	Config *ret = new Config();
	ret->alias = alias;

	for (std::vector<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++)
	{
		std::string bisEntry = std::string(*ite);
		ret->kv.push_back(bisEntry);
	}
	return ret;
}

bool Config::tryGetValue(const std::string &key, int &value)
{
	std::string val = _getValueForKey(key);
	if (val == "")
	{
		return false;
	}
	std::istringstream iss(val);
	if (!(iss >> value))
	{
		return false;
	}
	return true;

//	std::map<std::string, std::string>::iterator it = kv.find(key);
//	if (it != kv.end())
//	{
//		std::istringstream iss(it->second);
//		if (!(iss >> value))
//		{
//			return false;
//		}
//		return true;
//	}
//	return false;
}

//int Config::getRouteSpecificMaxBodySize(const std::string &route, int defaultSize)
//{
//	// Directly check for the specific route
//	if (route == "/post_body")
//	{
//		return 100; // Set specific limit for this route as required
//	}
//
//	// Otherwise, use the general settings from the configuration
//	std::string key = "maxBodySize:" + route; // Prefix to distinguish this type of config value
//	std::map<std::string, std::string>::iterator it = kv.find(key);
//	if (it != kv.end())
//	{
//		return std::atoi(it->second.c_str()); // Convert the string to int safely for C++98
//	}
//	return defaultSize; // Return the default size if no specific configuration is found
//}

