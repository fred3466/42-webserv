#include "ConfigReader.h"

ConfigReader::ConfigReader()
{
}

ConfigReader::~ConfigReader()
{
}

Config* findConfigByAlias(std::vector<Config*> v, std::string alias)
{
	Config *ret = NULL;
	std::vector<Config*>::iterator ite;
	for (ite = v.begin(); ite != v.end(); ite++)
	{
		if ((*ite)->getAlias() == alias)
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
	int lineNumber = 1;
	bool bValidated = true;

	std::string line;
	if (is.is_open())
	{
		while (std::getline(is, line))
		{
			is.clear();
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

			std::stringstream ss(line);
			std::getline(ss, fullKey, '=');
			su.ltrim(fullKey);
			if (fullKey.empty() || fullKey == "" || fullKey.length() == 0
					|| !fullKey.c_str())
			{
				continue;
			}
			std::getline(ss, val, '=');
			Config *c;
//			std::string alias;
			std::vector<std::string> toksVal = su.tokenize(val, ' ');
			std::string key = "";
//			std

//			SERVER
			if (fullKey == "server")
			{
				c = factory.build();
				ret->push_back(c);

//				toksVal = su.tokenize(val, ' ');

				std::string name = su.getNthTokenIfExists(toksVal, 0, "");
				c->addParam("name", name);
				c->addParam("alias", su.getNthTokenIfExists(toksVal, 1, name));

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
				continue;
			}
			else
			{
//				exemple : s1.listen=8080
				std::vector<std::string> toksFullKey = su.tokenize(fullKey,
						'.');
				std::string aliasKey = su.getNthTokenIfExists(toksFullKey, 0,
						"");
				key = su.getNthTokenIfExists(toksFullKey, 1, "");
				c = findConfigByAlias(*ret, aliasKey);
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
				std::string port = su.getNthTokenIfExists(toksVal, 1, "");
				c->addParam("port", port);
			}
			//			listen
			if (key == "qs")
			{
				c->addParam("listen",
						su.getNthTokenIfExists(toksVal, 0, "localhostttttt"));
				c->addParam("port", su.getNthTokenIfExists(toksVal, 1, ""));
			}

			harl.trace("%s -> %s", key.c_str(), val.c_str());
			lineNumber++;
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
