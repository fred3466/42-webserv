#include "ConfigValidator.h"

bool ConfigValidator::checkAlias(std::vector<Config*> v, Config *config,
		std::string aliasParam)
{
	std::vector<Config*>::iterator iteConf;
	for (iteConf = v.begin(); iteConf != v.end(); iteConf++)
	{
		std::string alias = config->getParamStr("alias", "");
		if (alias == aliasParam
				&& (*iteConf)->getId() != config->getId())
			return false;
	}
	return true;
}
