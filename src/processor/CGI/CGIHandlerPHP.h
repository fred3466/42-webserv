#pragma once

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../../request/API/Request.h"
#include "../../response/API/Response.h"
#include "CGIHandler.h"

class CGIHandlerPHP: public CGIHandler
{
private:
	Harl harl;
	std::string responseBody;
	std::map<std::string, std::string> responseHeaders;
	Config *config;

	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);

	void feedEnv(char **envp, std::map<std::string, std::string> envMap);
	virtual void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid);
	virtual void _childProcess(fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp);
	std::string buildCommandLine(const char *argv[], std::string interpreterPath, std::string &scriptPath);

public:
	CGIHandlerPHP();
	virtual ~CGIHandlerPHP();
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response);
	virtual std::string toString();
	virtual void setConfig(Config *conf);
};
