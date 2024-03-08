#pragma once
#include <vector>
#include "Config.h"

class ConfigValidator
{
public:
	bool checkAlias(std::vector<Config*> v, Config *config, std::string alias);
};

