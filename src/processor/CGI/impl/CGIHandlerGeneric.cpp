#include "CGIHandlerGeneric.h"
#include <map>

CGIHandlerGeneric::CGIHandlerGeneric() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper() {
}

CGIHandlerGeneric::~CGIHandlerGeneric() {
}

std::string CGIHandlerGeneric::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response) {
	cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return CGIHelper::getContent();
}

void CGIHandlerGeneric::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response) {
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerGeneric::buildCommandLine(Request *request, std::string interpreterPath, std::string &scriptPath) {
	(void) scriptPath;
	const char **argv = new const char*[3];
	int i = 0;

	std::string SCRIPT_NAME = request->getPath() + request->getFileName() + request->getFileExtension();

	argv[i] = new char[interpreterPath.length() + 1]();
	memcpy((char*) argv[i++], interpreterPath.c_str(), interpreterPath.length() + 1);
	std::string stp = SCRIPT_NAME;
	argv[i] = new char[stp.length() + 1]();
	memcpy((char*) argv[i++], stp.c_str(), stp.length() + 1);

	argv[i++] = NULL;

//	harl.debug("CGIHandlerGeneric::executeCGIScript [%s %s] ???", interpreterPath.c_str(), cmdLine.c_str());
	return argv;
}

std::string CGIHandlerGeneric::toString() {
	return "CGIHandlerGeneric";
}

void CGIHandlerGeneric::setConfig(Config *conf) {
	config = conf;
}
Config* CGIHandlerGeneric::getConfig() {
	return config;
}
