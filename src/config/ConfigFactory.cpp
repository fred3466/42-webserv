#include "ConfigFactory.h"

ConfigFactory::ConfigFactory()
{
}

ConfigFactory::~ConfigFactory()
{
}

Config *ConfigFactory::build()
{
	Config *conf = new Config();
	return conf;
}
