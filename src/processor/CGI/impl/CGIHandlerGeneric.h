#pragma once

#include "../../../util/FileUtil.h"
#include "../CGIHelper.h"

class CGIHandlerGeneric: public CGIHandler {
private:
	Harl harl;
	std::string responseBody;
	std::map<std::string, std::string> responseHeaders;
	Config *config;
	CGIHelper cgiHelper;

public:
	CGIHandlerGeneric();
	virtual ~CGIHandlerGeneric();
	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);
	virtual const char** buildCommandLine(Request *request, std::string interpreterPath, std::string &scriptPath);
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response);
	virtual std::string toString();
	virtual void setConfig(Config *conf);
	Config* getConfig();

};