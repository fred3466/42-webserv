#include "ConfigFactory.h"

ConfigFactory::ConfigFactory()
{
}

ConfigFactory::~ConfigFactory()
{
}

Config* ConfigFactory::build()
{
	Config *conf = new Config();
	conf->read("config.properties");
	return conf;
}
