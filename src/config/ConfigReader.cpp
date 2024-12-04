#include "ConfigReader.h"

ConfigReader::ConfigReader() {
}

ConfigReader::~ConfigReader() {
}

Config* findConfigByAlias(std::vector<Config*> *v, std::string alias) {
	Config *ret = NULL;
	std::vector<Config*>::iterator ite;
	for (ite = v->begin(); ite != v->end(); ite++) {
		std::string aliasTmp = (*ite)->getAlias();
		if (alias.compare(aliasTmp) == 0)
			ret = *ite;
	}
	return ret;
}

bool ConfigReader::buildConfigVector(std::vector<Config*> *ret, std::string path) {
	ConfigFactory factory = ConfigFactory();
	Harl harl = Harl();
	std::string fullKey, val;
	StringUtil su = StringUtil();
	FileUtil fu = FileUtil();
	std::ifstream is(path.c_str());

	ConfigValidator validator = ConfigValidator();
	int lineNumber = 0;
	bool bValidated = true;

	std::string line;
	std::string alias;

	if (is.is_open()) {
		while (std::getline(is, line)) {
			lineNumber++;
			is.clear();
			Config *c = NULL;
			std::vector<std::string> toksFullKey;
			if (is.peek() == '0' || line.empty()
					//					|| line.length() == 0
					//					|| !line.c_str()
					) {
				continue;
			}

			if (su.isCommented(line))
				continue;

			line = su.normalizeSpaces(line);
			harl.trace(line);

			su.trim(line);
			if (is.peek() == '0' || line.empty() || line + "" == "" || line.length() == 0 || !line.c_str()) {
				continue;
			}

			if (std::string::npos != line.find('{')) {
				std::string lineTmp = "";
				while (std::getline(is, lineTmp)) {
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
			if (fullKey.empty() || fullKey == "" || fullKey.length() == 0 || !fullKey.c_str()) {
				continue;
			}
			std::getline(ss, val, '=');
			std::vector<std::string> toksVal = su.tokenize(val, ' ');
			std::string key = "";
			//			std

			//			SERVER
			if (fullKey == "server") {
				c = factory.build();
				std::string webservPath = ".";
				c->addParam("ROOT_PATH", webservPath);
				if (Config::ROOT_PATH == "") {
					Config::ROOT_PATH = webservPath;
				}

				//				Validating
				// alias must be unique
				if ((alias != "") && (!validator.checkAlias(*ret, c, alias))) {
					bValidated = false;
					harl.error("Multiple occurrence of the same alias/name for [%s] in line %i", alias.c_str(), lineNumber);
				} else {
					ret->push_back(c);
					std::string name = su.getNthTokenIfExists(toksVal, 0, "");
					c->addParam("name", name);
					std::string alias = su.getNthTokenIfExists(toksVal, 1, "");
					c->setAlias(alias);
				}
				continue;
			} else {
				//				exemple : s1.listen=8080
				toksFullKey = su.tokenize(fullKey, '.');
				std::string aliasKey = su.getNthTokenIfExists(toksFullKey, 0, "");
				su.trim(aliasKey);
				key = su.getNthTokenIfExists(toksFullKey, 1, "");
				c = findConfigByAlias(ret, aliasKey);
			}

			alias = c->getAlias();

			//			base_path
			if (key == "base_path") {
				std::string base_path = su.getNthTokenIfExists(toksVal, 0, "");
				if (!fu.fileExists(base_path)) {
					harl.error("Directory not found for base_path=[%s] dans in line %i", base_path.c_str(), lineNumber);
					return false;
				} else {
					std::string pathCanonical = fu.realPathFile(base_path);
					c->addParam("base_path", base_path);
				}
			}
			//			server_name
			else if (key == "server_name") {
				c->addParam("server_name", su.getNthTokenIfExists(toksVal, 0, ""));
			}

			//			listen
			else if (key == "listen") {
				std::string ip = su.getNthTokenIfExists(toksVal, 0, "127.0.0.1");
				c->addParam("listen", ip);
				std::string port = su.getNthTokenIfExists(toksVal, 1, "80");
				c->addParam("port", port);
			}

			//			location
			else if (key == "location") {
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
			else if (key == "error_page") {
				std::string errorCode = su.getNthTokenIfExists(toksVal, 0, "");
				std::string errorFile = su.getNthTokenIfExists(toksVal, 1, "");
				c->addParam(errorCode + "_errorCodeFile", errorFile);

			} else {
				c->addParam(key, val);
			}

			harl.trace("ConfigReader::buildConfigVector: [%s] %s -> %s", alias.c_str(), key.c_str(), val.c_str());
		}
		is.close();
	}
	return bValidated;
}
