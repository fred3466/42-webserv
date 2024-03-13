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

bool ConfigReader::buildConfigVector(std::vector<Config*> *ret,
		std::string path)
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
			harl.info(line);

			su.trim(line);
			if (is.peek() == '0' || line.empty() || line + "" == ""
					|| line.length() == 0
					|| !line.c_str())
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
					harl.info(lineTmp);
					su.trim(line);
					line += lineTmp;
					if (std::string::npos != lineTmp.find('}'))
						break;
				}
			}

			std::stringstream ss(line);
			std::getline(ss, fullKey, '=');
			su.ltrim(fullKey);
			if (fullKey.empty() || fullKey == "" || fullKey.length() == 0
					|| !fullKey.c_str())
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

//				toksVal = su.tokenize(val, ' ');

//				Validating
				//alias must be unique
				std::string alias = c->getAlias();
				if (!validator.checkAlias(*ret, c, alias))
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
					std::string alias = su.getNthTokenIfExists(toksVal, 1, name);
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

//			root
			if (key == "root")
			{
				c->addParam("root", su.getNthTokenIfExists(toksVal, 0, ""));
			}
//			server_name
			if (key == "server_name")
			{
				c->addParam("server_name",
						su.getNthTokenIfExists(toksVal, 0, ""));
			}

//			listen
			if (key == "listen")
			{
				std::string ip = su.getNthTokenIfExists(toksVal, 0, "127.0.0.1");
				c->addParam("listen", ip);
				std::string port = su.getNthTokenIfExists(toksVal, 1, "8080");
				c->addParam("port", port);
			}

//			listen
			if (key == "location")
			{
				std::string urlPath = su.getNthTokenIfExists(toksFullKey, 2, "");
				c->addParam("location@" + urlPath, val);
			}

			harl.trace("%s -> %s", key.c_str(), val.c_str());
		}
		is.close();
	}
	return bValidated;
}

//Config
//ConfigReader::read(std::string path)
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
//}
