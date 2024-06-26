#include "ConfigReader.h"

ConfigReader::ConfigReader()
{
}

ConfigReader::~ConfigReader()
{
}

Config* findConfigByAlias(std::vector<Config*> *v, std::string alias)
{
	Config *ret = NULL;
	std::vector<Config*>::iterator ite;
	for (ite = v->begin(); ite != v->end(); ite++)
	{
		std::string aliasTmp = (*ite)->getAlias();
		if (alias.compare(aliasTmp) == 0)
			ret = *ite;
	}
	return ret;
}

bool ConfigReader::buildConfigVector(std::vector<Config*> *ret, std::string path)
{
	ConfigFactory factory = ConfigFactory();
	Harl harl = Harl();
	std::string fullKey, val;
	StringUtil su = StringUtil();
	std::ifstream is(path.c_str());
	//	TODO et la factory alors?
	ConfigValidator validator = ConfigValidator();
	int lineNumber = 0;
	bool bValidated = true;

	std::string line;
	if (is.is_open())
	{
		while (std::getline(is, line))
		{
			lineNumber++;
			is.clear();
			Config *c = NULL;
			std::vector<std::string> toksFullKey;
			if (is.peek() == '0' || line.empty()
					//					|| line.length() == 0
					//					|| !line.c_str()
					)
			{
				continue;
			}

			if (su.isCommented(line))
				continue;

			line = su.normalizeSpaces(line);
			harl.trace(line);

			su.trim(line);
			if (is.peek() == '0' || line.empty() || line + "" == "" || line.length() == 0 || !line.c_str())
			{
				continue;
			}

			if (std::string::npos != line.find('{'))
			{
				std::string lineTmp = "";
				while (std::getline(is, lineTmp))
				{
					lineNumber++;
					if (su.isCommented(lineTmp))
						continue;
					lineTmp = su.normalizeSpaces(lineTmp);
					harl.trace(lineTmp);
					line = su.trim(line);
					line += lineTmp;
					if (std::string::npos != lineTmp.find('}'))
						break;
				}
			}

			std::stringstream ss(line);
			std::getline(ss, fullKey, '=');
			su.ltrim(fullKey);
			if (fullKey.empty() || fullKey == "" || fullKey.length() == 0 || !fullKey.c_str())
			{
				continue;
			}
			std::getline(ss, val, '=');
			//			std::string alias;
			std::vector<std::string> toksVal = su.tokenize(val, ' ');
			std::string key = "";
			//			std

			//			SERVER
			if (fullKey == "server")
			{
				c = factory.build();
				char webservPath[1024];
				getcwd(webservPath, 1024);
				c->addParam("ROOT_PATH", webservPath);

				//				toksVal = su.tokenize(val, ' ');

				//				Validating
				// alias must be unique
				std::string alias = c->getAlias();
				if ((alias != "") && (!validator.checkAlias(*ret, c, alias)))
				{
					bValidated = false;
					harl.error(
							"Multiple occurrence of the same alias/name for [%s] in line %i",
							alias.c_str(), lineNumber);
				}
				else
				{
					ret->push_back(c);
					std::string name = su.getNthTokenIfExists(toksVal, 0, "");
					c->addParam("name", name);
					std::string alias = su.getNthTokenIfExists(toksVal, 1, "");
					c->setAlias(alias);
				}
				continue;
			}
			else
			{
				//				exemple : s1.listen=8080
				toksFullKey = su.tokenize(fullKey, '.');
				std::string aliasKey = su.getNthTokenIfExists(toksFullKey, 0, "");
				su.trim(aliasKey);
				key = su.getNthTokenIfExists(toksFullKey, 1, "");
				c = findConfigByAlias(ret, aliasKey);
			}

			//			base_path
			if (key == "base_path")
			{
				std::string base_path = su.getNthTokenIfExists(toksVal, 0, "");
				char *path = realpath(base_path.c_str(), NULL);
				if (path == NULL)
				{
					//					TODO traiter l'erreur de configuration et abandonner le chargement (erreur fatale)!
				}
				else
				{
//					std::string real_base_path(path);
					c->addParam("base_path", base_path);
//					c->addParam("real_base_path", real_base_path);
					free(path);
				}
			}
			//			server_name
			else if (key == "server_name")
			{
				c->addParam("server_name",
						su.getNthTokenIfExists(toksVal, 0, ""));
			}

			//			listen
			else if (key == "listen")
			{
				std::string ip = su.getNthTokenIfExists(toksVal, 0, "127.0.0.1");
				c->addParam("listen", ip);
				std::string port = su.getNthTokenIfExists(toksVal, 1, "80");
				c->addParam("port", port);
			}

			//			location
			else if (key == "location")
			{
				std::string urlPath = su.getNthTokenIfExists(toksFullKey, 2, "");
				std::size_t ite = val.find('{');
				if (ite != std::string::npos)
					val.erase(ite, 1);
				ite = val.find('}');
				if (ite != std::string::npos)
					val.erase(ite, 1);

				c->addParam("location@" + urlPath, val);
			}
			//			error_page
			else if (key == "error_page")
			{
				std::string errorCode = su.getNthTokenIfExists(toksVal, 0, "");
				std::string errorFile = su.getNthTokenIfExists(toksVal, 1, "");
				c->addParam(errorCode + "_errorCodeFile", errorFile);

			}
			else
			{
				c->addParam(key, val);
			}

			harl.trace("ConfigReader::buildConfigVector: %s -> %s", key.c_str(), val.c_str());
		}
		is.close();
	}
	return bValidated;
}

// Config *findConfigByAlias(std::vector<Config *> *v, std::string alias)
// {
// 	for (auto &config : *v)
// 	{
// 		if (config->getAlias() == alias)
// 		{
// 			return config;
// 		}
// 	}
// 	return NULL;
// }

// bool ConfigReader::buildConfigVector(std::vector<Config *> *ret, std::string path)
// {
// 	ConfigFactory factory;
// 	Harl harl;
// 	StringUtil su;
// 	std::ifstream is(path.c_str());
// 	ConfigValidator validator;
// 	int lineNumber = 0;
// 	bool bValidated = true;

// 	std::string line;
// 	if (is.is_open())
// 	{
// 		while (std::getline(is, line))
// 		{
// 			lineNumber++;
// 			// No need to clear the std::ifstream here in this context.
// 			if (line.empty() || su.isCommented(line))
// 			{
// 				continue; // Skip empty or commented lines early.
// 			}

// 			line = su.normalizeSpaces(line);
// 			harl.info(line);

// 			std::stringstream ss(line); // Using std::stringstream from C++98
// 			std::string fullKey, val;
// 			std::getline(ss, fullKey, '=');
// 			fullKey = su.ltrim(fullKey); // Assuming ltrim returns the trimmed string.
// 			if (fullKey.empty())
// 			{
// 				continue; // Skip if the key is empty after trim.
// 			}

// 			if (std::getline(ss, val, '='))
// 			{
// 				val = su.trim(val); // Assuming trim returns the trimmed string.

// 				if (fullKey == "server")
// 				{
// 					Config *c = factory.build(); // Properly define the Config object here
// 					std::string alias = su.getAliasFromVal(val);
// 					if (!validator.checkAlias(*ret, c, alias))
// 					{
// 						bValidated = false;
// 						harl.error("Duplicate server alias [%s] found in line %i", alias.c_str(), lineNumber);
// 						delete c; // Clean up to avoid memory leak
// 						continue;
// 					}
// 					// After validation passes, use the alias for further configuration setup
// 					c->setAlias(alias); // Make sure setAlias is called before pushing to vector
// 					ret->push_back(c);
// 				}
// 				else
// 				{
// 					// Assuming 'c' is previously defined and valid here for processing other keys
// 					// Process other configurations.
// 				}
// 			}
// 		}
// 		is.close();
// 	}
// 	return bValidated;
// }

// Config
// ConfigReader::read(std::string path)
//{
//	Config config = Config();
//	Harl harl = Harl();
//	std::string key, val;
//
//	std::ifstream is(path.c_str());
//
//	std::string line;
//	if (is.is_open())
//	{
//		while (std::getline(is, line))
//		{
//			std::stringstream ss(line);
//			std::getline(ss, key, '=');
//			std::getline(ss, val, '=');
//			config.addParam(key, val);
//			harl.debug("%s -> %s", key.c_str(), val.c_str());
//		}
//		is.close();
//	}
//	return config;
// }
