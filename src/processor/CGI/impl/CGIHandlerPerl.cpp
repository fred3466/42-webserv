#include "CGIHandlerPerl.h"
#include <map>

CGIHandlerPerl::CGIHandlerPerl() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper()
{
}

CGIHandlerPerl::~CGIHandlerPerl()
{
//	delete config;
}

std::string CGIHandlerPerl::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request, Response *response)
{
	std::string ret = cgiHelper.executeCGIScript(this, interpreterPath, scriptPath, request, response);
	return ret;
}

void CGIHandlerPerl::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)
{
	cgiHelper.setupEnvironmentVariables(envMap, request, response);
}

const char** CGIHandlerPerl::buildCommandLine(std::string interpreterPath, std::string &scriptPath)
{
	const char **argv = new const char*[4];
	int i = 0;
	argv[i++] = interpreterPath.c_str();
	argv[i++] = "-U";
	std::string stp = scriptPath;
	//			TODO leak
	//			TODO BUG!!!!
	argv[i] = new char[stp.length() + 1]();
	memcpy((char*) argv[i++], stp.c_str(), stp.length() + 1);

	//	TODO ne fonctionne plus ?
//	argv[i++] = stp.c_str();
	argv[i++] = NULL;

//	harl.debug("CGIHandlerPerl::executeCGIScript [%s %s] ???", interpreterPath.c_str(), cmdLine.c_str());
	return argv;
}

std::string CGIHandlerPerl::toString()
{
	return "CGIHandlerPerl";
}

void CGIHandlerPerl::setConfig(Config *conf)
{
	config = conf;
}
Config* CGIHandlerPerl::getConfig()
{
	return config;
}
