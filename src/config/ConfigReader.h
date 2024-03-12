#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Config.h"
#include "../util/StringUtil.h"
#include "ConfigValidator.h"
#include "ConfigFactory.h"

class ConfigReader
{
public:
	ConfigReader();
	virtual ~ConfigReader();

	bool buildConfigVector(std::vector<Config*> *ret, std::string path);

private:
//	Config read(std::string path);
};
