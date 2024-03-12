#include "ConfigValidator.h"

bool ConfigValidator::checkAlias(std::vector<Config*> v, Config *config,
		std::string aliasParam)
{
	std::vector<Config*>::iterator iteConf;
	for (iteConf = v.begin(); iteConf != v.end(); iteConf++)
	{
		std::string alias = config->getParamStr("alias", "");
		if (alias == aliasParam
				&& (*iteConf)->getAlias() != config->getAlias())
			return false;
	}
	return true;
}
