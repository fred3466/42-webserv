#include "ConfigValidator.h"

bool ConfigValidator::checkAlias(std::vector<Config *> v, Config *config,
								 std::string aliasParam)
{
	std::vector<Config *>::iterator iteConf;
	for (iteConf = v.begin(); iteConf != v.end(); iteConf++)
	{
		std::string alias = config->getParamStr("alias", "");
		if (alias == aliasParam && (*iteConf)->getAlias() != config->getAlias())
			return false;
	}
	return true;
}

// bool ConfigValidator::checkAlias(std::vector<Config *> v, Config *config, std::string aliasParam)
// {
// 	for (std::vector<Config *>::iterator it = v.begin(); it != v.end(); ++it)
// 	{
// 		Config *existingConfig = *it;

// 		if (existingConfig->getAlias() == aliasParam)
// 		{
// 			if (existingConfig != config)
// 			{
// 				return false;
// 			}
// 		}
// 	}
// 	// If no duplicate alias found or it's the same config, return true.
// 	return true;
// }