#pragma once

#include <string>
#include <map>

#include "../../request/API/Request.h"
#include "../../response/API/Response.h"
#include "../../Harl.h"

class CGIHandler
{
private:
	Harl harl;
	// Parses the CGI script output
	void parseOutput(const std::string &output);

	std::string responseBody;
	std::map<std::string, std::string> responseHeaders;
	void setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request,
			Response *response);

	std::string captureScriptOutput(int fileDescriptor);

public:
	CGIHandler();
	virtual ~CGIHandler();

	// Executes the CGI script and returns the result
	std::string executeCGI(const std::string &scriptPath, const std::map<std::string, std::string> &envVariables);
	void logError(const std::string &message);
	void logSuccess(const std::string &message);
	//    std::string executeCGIScript(const std::string &scriptPath);
	std::string executeCGIScript(std::string interpreterPath, std::string &scriptPath,
			Request *request, Response *response);
};

// export REQUEST_METHOD=GET
// export QUERY_STRING="param1=value1&param2=value2"
// export REDIRECT_STATUS=200
// php-cgi /home/parallels/Desktop/Parallels Shared Folders/42/webserv/src/cgi-bin/show_request_info.php

// /etc/php/8.1/cli php.ini
// cgi.force_redirect = 0
