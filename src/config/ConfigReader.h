#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include "Config.h"
#include "../util/FileUtil.h"
#include "../util/StringUtil.h"
#include "../Harl.h"
#include "ConfigValidator.h"
#include "ConfigFactory.h"
#include "unistd.h"

class ConfigReader {
public:
	ConfigReader();
	virtual ~ConfigReader();

	bool buildConfigVector(std::vector<Config*> *ret, std::string path);

private:
};
