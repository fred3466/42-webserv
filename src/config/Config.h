#pragma once
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include "../Harl.h"
#include "../util/StringUtil.h"

class Config
{
private:
	//	std::map<std::string, std::string> kv;
	std::vector<std::string> kv;
	std::string alias;
	std::string _getValueForKey(std::string key);
	bool _deleteEntryForKey(std::string key);

public:
	Config();
	~Config();
	Config(Config &bis);
	Config& operator=(Config &bis);
	std::string getParamStr(std::string param, std::string stringDefault);
	std::vector<std::string>* getParamStrStartingWith(std::string paramPrefix);
	int getParamInt(std::string param, int intDefault);
	int getParamInt(const std::string &key);
	void addParam(std::string param, std::string value);
	void addOrReplaceParam(std::string param, std::string value);
	//	void setId(int id);
	std::string getAlias();
	void setAlias(std::string alias);
	Config* clone();
	bool tryGetValue(const std::string &key, int &value);
//	int getRouteSpecificMaxBodySize(const std::string &route, int defaultSize);
};
