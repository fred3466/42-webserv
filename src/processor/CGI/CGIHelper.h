#pragma once
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../../API/CGIHandler.h"
#include "../../API/Request.h"
#include "../../API/Response.h"
#include "../../Harl.h"
#include "../../util/FileUtil.h"
// Structure avec des tubes full-duplex
typedef struct fdpipe
{
	int fd_for_REQUEST[2];
	int fd_for_RESPONSE[2];
} fdpipe;

class CGIHelper
{
private:
	Harl harl;

public:
	CGIHelper();
	~CGIHelper();

	void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response);
	void feedEnv(char **envp, std::map<std::string, std::string> envMap);
	void _childProcess(CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp);
	void _parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid);
	std::string executeCGIScript(CGIHandler *cgiHandler, std::string interpreterPath, std::string &scriptPath, Request *request, Response *response);
};
