#pragma once
#include <string>
#include <map>
#include <stdexcept>
#include "../Harl.h"

class Config
{
private:
	std::map<std::string, std::string> kv;
	static int idRef;
	int id;

public:
	Config();
	~Config();
	Config(Config &bis);
	Config& operator=(Config &bis);
	std::string getParamStr(std::string param, std::string stringDefault);
	int getParamInt(std::string param, int intDefault);
	void addParam(std::string param, std::string value);

//	void setId(int id);
	int getId();
};
