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
#include "CGIHandler.h"

class CGIHandlerLeg: public CGIHandler
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

public:
	CGIHandlerLeg();
	virtual ~CGIHandlerLeg();
	virtual std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response);
	virtual std::string toString();
	virtual void setConfig(Config *conf);
};

// export REQUEST_METHOD=GET
// export QUERY_STRING="param1=value1&param2=value2"
// export REDIRECT_STATUS=200
// php-cgi /home/parallels/Desktop/Parallels Shared Folders/42/webserv/src/cgi-bin/show_request_info.php

// /etc/php/8.1/cli php.ini
// cgi.force_redirect = 0
