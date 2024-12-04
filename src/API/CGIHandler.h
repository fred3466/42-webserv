#pragma once

#include <string>
#include <map>

#include "Request.h"
#include "Response.h"
#include "../config/Config.h"

class CGIHandler {

public:
	CGIHandler();
	virtual ~CGIHandler();
	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)=0;
	virtual const char** buildCommandLine(Request *request, std::string interpreterPath, std::string &scriptPath)=0;
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response)=0;
	virtual std::string toString()=0;
	virtual void setConfig(Config *conf) = 0;
	virtual Config* getConfig() = 0;
};
