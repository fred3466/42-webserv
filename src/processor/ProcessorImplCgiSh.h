#pragma once

#include <map>
#include "Processor.h"
#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/API/Response.h"
#include "../response/factory/ResponseFactory.h"

#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "CGI/CGIHandler.h"
#include "../location/ProcessorAndLocationToProcessor.h"

class ProcessorImplCgiSh: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;
	std::map<std::string, std::string> prepareCGIEnvironment(Request *request);
	ProcessorTypeEnum type;

protected:

public:
	ProcessorImplCgiSh(ProcessorTypeEnum type);
	~ProcessorImplCgiSh();
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual ProcessorTypeEnum getType();

	std::string readRequest(int clientFd);
	void sendResponse(int clientFd, const std::string &response);
	bool isCGIRequest(const std::string &uri);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
};
