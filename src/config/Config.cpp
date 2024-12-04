#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string Config::ROOT_PATH = "";

Config::Config() : kv(), alias("") {
}

Config::~Config() {
	Harl().debug("Config::~Config : kv : %i", kv.size());
	if (kv.size() > 0)
		kv.erase(kv.begin(), kv.end());
}

bool Config::_deleteEntryForKey(std::string key) {
	std::string ret = "";
	StringUtil su = StringUtil();
	std::string keyUpper = su.strUpperCase(key);

	for (std::list<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++) {
		std::string rawEntry = *ite;
		std::vector<std::string> toks = su.tokenize(rawEntry, '|');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		name = su.strUpperCase(su.trim(name));
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		val = su.trim(val);
		if (name == keyUpper) {
			kv.erase(ite);
			return true;
		}
	}
	return false;
}

std::string Config::_getValueForKey(std::string key) {
	std::string ret = "";
	StringUtil su = StringUtil();
	std::string keyUpper = su.strUpperCase(key);

	for (std::list<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++) {
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

int Config::getParamInt(std::string param, int intDefault) {
	try {
		std::string res = _getValueForKey(param);

		if (!res.empty()) {
			int resInt = StringUtil().intFromStr(res);
			return resInt;
		}
	} catch (const std::out_of_range &oor) {
		//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return intDefault;
}

int Config::getParamInt(const std::string &key) {
	int value;
	if (!tryGetValue(key, value)) {
		throw std::runtime_error("Required configuration parameter missing: " + key);
	}
	return value;
}

void Config::addOrReplaceParam(std::string param, std::string val) {
	_deleteEntryForKey(param);

	kv.push_back(param + "|" + val);
}

void Config::addParam(std::string param, std::string value) {
	param.append("|").append(value);
	kv.push_back(param);
}

std::string Config::getParamStr(std::string param, std::string stringDefault) {
	try {
		std::string res = _getValueForKey(param);
		if (!res.empty()) {
			return res;
		}
	} catch (const std::out_of_range &oor) {
		//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return stringDefault;
}

std::vector<std::string>* Config::getParamStrStartingWith(std::string paramPrefix) {
	StringUtil su = StringUtil();
	std::vector<std::string> *ret = new std::vector<std::string>();

	for (std::list<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++) {
		std::string rawEntry = *ite;

		std::vector<std::string> toks = su.tokenize(rawEntry, '|');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		if (name.find(paramPrefix) == 0) {
			ret->push_back(rawEntry);
		}
	}

	return ret;
}

std::string Config::getAlias() {
	return alias;
}

void Config::setAlias(std::string alias) {
	this->alias = alias;
}

Config::Config(Config &bis) : alias(bis.alias) {
	for (std::list<std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++) {
		std::string bisEntry = std::string(*ite);
		kv.push_back(bisEntry);
	}
	if (this != &bis)
		*this = bis;
}

Config& Config::operator=(Config &bis) {
	this->alias = bis.alias;

	for (std::list<std::string>::iterator ite = bis.kv.begin(); ite != bis.kv.end(); ite++) {
		std::string bisEntry = std::string(*ite);
		kv.push_back(bisEntry);
	}
	return *this;
}

Config* Config::clone() {
	Config *ret = new Config();
	ret->alias = alias;

	for (std::list<std::string>::iterator ite = kv.begin(); ite != kv.end(); ite++) {
		std::string bisEntry = std::string();
		bisEntry.assign(*ite);
		ret->kv.push_back(bisEntry);
	}
	return ret;
}

bool Config::tryGetValue(const std::string &key, int &value) {
	std::string val = _getValueForKey(key);
	if (val == "") {
		return false;
	}
	std::istringstream iss(val);
	if (!(iss >> value)) {
		return false;
	}
	return true;
}
