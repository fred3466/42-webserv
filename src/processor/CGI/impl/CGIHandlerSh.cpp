#include "CGIHandlerSh.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>

CGIHandlerSh::CGIHandlerSh() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper()
{
}

CGIHandlerSh::~CGIHandlerSh()
{
}

std::string CGIHandlerSh::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response)
{
	std::string ret = cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return ret;
}

void CGIHandlerSh::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)
{
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerSh::buildCommandLine(std::string interpreterPath, std::string &scriptPath)
{
	const char **argv = new const char*[10];
	int i = 0;
	argv[i++] = (char*) (interpreterPath.c_str());
	std::string stp = scriptPath;
	argv[i++] = (char*) stp.c_str();
	argv[i++] = NULL;

//	*cmdLine = StringUtil().fromCArrayToString(argv);
	return argv;
}

std::string CGIHandlerSh::toString()
{
	return "CGIHandlerSh";
}

void CGIHandlerSh::setConfig(Config *conf)
{
	config = conf;
}
Config* CGIHandlerSh::getConfig()
{
	return config;
}
