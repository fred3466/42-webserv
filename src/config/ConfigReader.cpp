#include "ConfigReader.h"

ConfigReader::ConfigReader()
{
}

ConfigReader::~ConfigReader()
{
}
std::vector<std::string> tokenize(std::string s)
{
	std::vector<std::string> ret = std::vector<std::string>();
	std::stringstream ss(s);
	std::string word;
	while (ss >> word)
	{
		ret.push_back(word);
	}
	return ret;
}

std::vector<std::string> tokenize(std::string s, char sep)
{
	std::vector<std::string> ret = std::vector<std::string>();
	std::stringstream ss(s);
	std::string word;
	while (!ss.eof())
	{
		getline(ss, word, sep);
		ret.push_back(word);
	}
	return ret;
}

std::string getNthTokenIfExists(std::vector<std::string> v, int index,
		std::string defaultValue)
{
	if (v.size() > index)
		return v[index];
	else
		return defaultValue;
}

Config* findConfigById(std::vector<Config*> v, int id)
{
	Config *ret = NULL;
	std::vector<Config*>::iterator ite;
	for (ite = v.begin(); ite != v.end(); ite++)
	{
		if ((*ite)->getId() == id)
			ret = *ite;
	}
	return ret;
}

bool ConfigReader::buildConfigVector(std::vector<Config*> ret,
		std::string path)
{
	ConfigFactory factory = ConfigFactory();
	Harl harl = Harl();
	std::string key, val;
	StringUtil stringUtil = StringUtil();
	std::ifstream is(path.c_str());
	int configId;
//	TODO et la factory alors?
	ConfigValidator validator = ConfigValidator();
	int lineNumber = 1;
	bool bValidated = true;

	std::string
	line;
	if (is.is_open())
	{
		while (std::getline(is, line))
		{
			if (stringUtil.isCommented(line))
				continue;

			line = stringUtil.normalizeSpaces(line);
			harl.info(line);

			std::stringstream ss(line);
			std::getline(ss, key, '=');
			stringUtil.ltrim(key);
			std::getline(ss, val, '=');

//			SERVER
			if (key == "server")
			{
				Config *c = factory.build();
				ret.push_back(c);

				std::vector<std::string> toks = tokenize(val);
				std::string name = getNthTokenIfExists(toks, 0, "");
				c->addParam("name", name);
				c->addParam("alias", getNthTokenIfExists(toks, 1, name));

//				Validating
				//alias must be unique
				std::string alias = c->getParamStr("alias", "");
				if (!validator.checkAlias(ret, c, alias))
				{
					bValidated = false;
					harl.error(
							"Multiple occurrence of the same alias/name for [%s] in line %i",
							alias.c_str(), lineNumber);
				}
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
