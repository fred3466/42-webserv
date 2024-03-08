#pragma once
#include "Config.h"

class ConfigFactory
{
public:
	ConfigFactory();
	~ConfigFactory();
	Config *build();
};
