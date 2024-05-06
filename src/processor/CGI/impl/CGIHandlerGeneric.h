#pragma once

#include "../../../API/Request.h"
#include "../../../API/Response.h"
#include "../../../util/FileUtil.h"
#include "../CGIHelper.h"

class CGIHandlerGeneric: public CGIHandler
{
private:
	Harl harl;
	std::string responseBody;
	std::map<std::string, std::string> responseHeaders;
	Config *config;
	CGIHelper cgiHelper;

//	void feedEnv(char **envp, std::map<std::string, std::string> envMap);
//	virtual void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid);
//	virtual void _childProcess(fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp);

public:
	CGIHandlerGeneric();
	virtual ~CGIHandlerGeneric();
	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);
	virtual const char** buildCommandLine(std::string interpreterPath, std::string &scriptPath);
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response);
	virtual std::string toString();
	virtual void setConfig(Config *conf);
	Config* getConfig();

};
