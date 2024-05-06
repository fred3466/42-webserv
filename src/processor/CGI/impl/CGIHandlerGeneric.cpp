#include "CGIHandlerGeneric.h"
#include <map>

CGIHandlerGeneric::CGIHandlerGeneric() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper()
{
}

CGIHandlerGeneric::~CGIHandlerGeneric()
{
//	delete config;
}

std::string CGIHandlerGeneric::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response)
{
	std::string ret = cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return ret;
}

void CGIHandlerGeneric::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)
{
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerGeneric::buildCommandLine(std::string interpreterPath, std::string &scriptPath)
{
	const char **argv = new const char*[3];
	int i = 0;
	argv[i++] = interpreterPath.c_str();
	std::string stp = scriptPath;
	//			TODO new
	argv[i] = new char[stp.length() + 1]();
	memcpy((char*) argv[i++], stp.c_str(), stp.length() + 1);

	argv[i++] = NULL;

//	harl.debug("CGIHandlerGeneric::executeCGIScript [%s %s] ???", interpreterPath.c_str(), cmdLine.c_str());
	return argv;
}

std::string CGIHandlerGeneric::toString()
{
	return "CGIHandlerGeneric";
}

void CGIHandlerGeneric::setConfig(Config *conf)
{
	config = conf;
}
Config* CGIHandlerGeneric::getConfig()
{
	return config;
}
