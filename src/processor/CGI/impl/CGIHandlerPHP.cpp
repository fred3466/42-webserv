#include "CGIHandlerPHP.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>

CGIHandlerPHP::CGIHandlerPHP() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper()
{
}

CGIHandlerPHP::~CGIHandlerPHP()
{
//	delete config;
}

std::string CGIHandlerPHP::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response)
{
	std::string ret = cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return ret;
}

void CGIHandlerPHP::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)
{
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerPHP::buildCommandLine(std::string interpreterPath, std::string &scriptPath)
{
	const char **argv = new const char*[10];
	int i = 0;
	argv[i++] = (char*) (interpreterPath.c_str());
	argv[i++] = (char*) "-c";
	argv[i++] = (char*) "/home/fbourgue/0-wsp/webserv/php.ini";
	argv[i++] = (char*) "-d";
	argv[i++] = (char*) "root_dir=/home/fbourgue/0-wsp/webserv/cgi-bin";
	argv[i++] = (char*) "-d";
	argv[i++] = (char*) "cgi.force_redirect=0";
	std::string stp = scriptPath;
//		std::string stp = "\"" + scriptPath + "\"";
	argv[i++] = (char*) "-f";
	argv[i++] = (char*) stp.c_str();
	argv[i++] = NULL;

//	*cmdLine = StringUtil().fromCArrayToString(argv);
	return argv;
}

std::string CGIHandlerPHP::toString()
{
	return "CGIHandlerPHP";
}

void CGIHandlerPHP::setConfig(Config *conf)
{
	config = conf;
}
Config* CGIHandlerPHP::getConfig()
{
	return config;
}
