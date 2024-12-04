#include "CGIHandlerPerl.h"
#include <map>

CGIHandlerPerl::CGIHandlerPerl() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper() {
}

CGIHandlerPerl::~CGIHandlerPerl() {
}

std::string CGIHandlerPerl::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response) {
	cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return CGIHelper::getContent();
}

void CGIHandlerPerl::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response) {
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerPerl::buildCommandLine(Request *request, std::string interpreterPath, std::string &scriptPath) {
	(void) request;
	char **argv = new char*[4];
	int i = 0;

	argv[i] = new char[interpreterPath.length() + 1]();
	memcpy((char*) argv[i++], interpreterPath.c_str(), interpreterPath.length() + 1);

	std::string tiretU = "-U";
	argv[i] = new char[tiretU.length() + 1]();
	memcpy((char*) argv[i++], tiretU.c_str(), tiretU.length() + 1);

	std::string stp = scriptPath;
	argv[i] = new char[stp.length() + 1]();
	memcpy((char*) argv[i++], stp.c_str(), stp.length() + 1);

	argv[i] = NULL;

	return (const char**) argv;
}

std::string CGIHandlerPerl::toString() {
	return "CGIHandlerPerl";
}

void CGIHandlerPerl::setConfig(Config *conf) {
	config = conf;
}
Config* CGIHandlerPerl::getConfig() {
	return config;
}
