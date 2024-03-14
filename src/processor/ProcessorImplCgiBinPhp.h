#pragma once

#include <map>
#include "Processor.h"
#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"

#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/factory/ResponseFactory.h"
#include "CGI/CGIHandler.h"

class ProcessorImplCgiBinPhp: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;
	//	std::map<std::string, std::string> env;

	std::map<std::string, std::string> prepareCGIEnvironment(Request *request);
	protected:

public:
	ProcessorImplCgiBinPhp();
	~ProcessorImplCgiBinPhp();
	virtual Response* process(Request *request, Response *response);
	virtual void setConfig(Config *conf);

	std::string readRequest(int clientFd);
	void sendResponse(int clientFd, const std::string &response);
	//	void closeClient(int clientFd);
	//	int getListenFd();
	bool isCGIRequest(const std::string &uri);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
//	int getClientFd(int clientId);

};

