#pragma once

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../Harl.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "Request.h"
#include "Response.h"

class CGIHandler
{
	//	virtual void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid)=0;
//	virtual void _childProcess(fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp)=0;

public:
	CGIHandler();
	virtual ~CGIHandler();
	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)=0;
	virtual const char** buildCommandLine(std::string interpreterPath, std::string &scriptPath)=0;
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response)=0;
	virtual std::string toString()=0;
	virtual void setConfig(Config *conf) = 0;
	virtual Config* getConfig() = 0;
};
