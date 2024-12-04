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

CGIHandlerSh::CGIHandlerSh() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper() {
}

CGIHandlerSh::~CGIHandlerSh() {
}

std::string CGIHandlerSh::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response) {
	cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return CGIHelper::getContent();
}

void CGIHandlerSh::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response) {
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerSh::buildCommandLine(Request *request, std::string interpreterPath, std::string &scriptPath) {
	(void) request;
	const char **argv = new const char*[3];
	int i = 0;

	argv[i] = new char[interpreterPath.length() + 1]();
	memcpy((char*) argv[i++], interpreterPath.c_str(), interpreterPath.length() + 1);

	std::string stp = scriptPath;
	argv[i] = new char[stp.length() + 1]();
	memcpy((char*) argv[i++], stp.c_str(), stp.length() + 1);

	argv[i++] = NULL;

	return argv;
}

std::string CGIHandlerSh::toString() {
	return "CGIHandlerSh";
}

void CGIHandlerSh::setConfig(Config *conf) {
	config = conf;
}
Config* CGIHandlerSh::getConfig() {
	return config;
}
