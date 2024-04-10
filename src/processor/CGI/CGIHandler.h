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
#include "../../Harl.h"
#include "../../util/StringUtil.h"
#include "../../config/Config.h"

// Structure avec des tubes full-duplex
typedef struct fdpipe
{
	int fd_for_REQUEST[2];
	int fd_for_RESPONSE[2];
} fdpipe;

class CGIHandler
{
private:
	virtual void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)=0;
	virtual void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid)=0;
	virtual void _childProcess(fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp)=0;

public:
	CGIHandler();
	virtual ~CGIHandler();
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response)=0;
	virtual std::string toString()=0;
	virtual void setConfig(Config *conf) = 0;
};
