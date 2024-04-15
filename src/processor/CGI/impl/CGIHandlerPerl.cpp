#include "CGIHandlerPerl.h"

CGIHandlerPerl::CGIHandlerPerl() : harl(), responseBody(""), responseHeaders(), config(), cgiHelper()
{
}

CGIHandlerPerl::~CGIHandlerPerl()
{
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
//	argv[i++] = "/home/fbourgue/0-wsp/webserv/php.ini";
//	argv[i++] = "-d";
//	argv[i++] = "root_dir=/home/fbourgue/0-wsp/webserv/cgi-bin";
//	argv[i++] = "-d";
//	argv[i++] = "cgi.force_redirect=0";
	std::string stp = scriptPath;
//		std::string stp = "\"" + scriptPath + "\"";
//	argv[i++] = "-f";
	argv[i++] = stp.c_str();
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
