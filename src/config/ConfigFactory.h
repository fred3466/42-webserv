#pragma once
#include "Config.h"

class ConfigFactory
{
public:
	ConfigFactory();
	~ConfigFactory();
	//	ConfigFactory(const ConfigFactory &other);
//	ConfigFactory& operator=(const ConfigFactory &other);
	Config* build();
};

