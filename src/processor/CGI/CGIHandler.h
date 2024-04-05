#pragma once

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

// Structure avec des tubes full-duplex
typedef struct fdpipe
{
	int fd[2];
	int fds[2];
} fdpipe;

class CGIHandler
{
private:
	Harl harl;
	std::string responseBody;
	std::map<std::string, std::string> responseHeaders;

	void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);

	void feedEnv(char **envp, std::map<std::string, std::string> envMap);
	void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid);
	void _childProcess(fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request);

public:
	CGIHandler();
	virtual ~CGIHandler();
	std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response);
};

// export REQUEST_METHOD=GET
// export QUERY_STRING="param1=value1&param2=value2"
// export REDIRECT_STATUS=200
// php-cgi /home/parallels/Desktop/Parallels Shared Folders/42/webserv/src/cgi-bin/show_request_info.php

// /etc/php/8.1/cli php.ini
// cgi.force_redirect = 0
