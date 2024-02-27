/*
 * ConfigFactory.cpp
 *
 *  Created on: 24 févr. 2024
 *      Author: fbourgue
 */

#include "ConfigFactory.h"

ConfigFactory::ConfigFactory()
{
	// TODO Auto-generated constructor stub

}

ConfigFactory::~ConfigFactory()
{
	// TODO Auto-generated destructor stub
}

//ConfigFactory::ConfigFactory(const ConfigFactory &other)
//{
//	// TODO Auto-generated constructor stub
//
//}
//
//ConfigFactory& ConfigFactory::operator=(const ConfigFactory &other)
//{
//	// TODO Auto-generated method stub
//
//}
Config* ConfigFactory::build()
{
	Config *conf = new Config();
	conf->read("config.properties");
	return conf;
}
