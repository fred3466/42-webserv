#pragma once

#include <map>
#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "Processor.h"
#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/factory/ResponseFactory.h"
#include "CGI/CGIHandler.h"
#include "../location/ProcessorAndLocationToProcessor.h"

class LocationToProcessor;

class ProcessorImplCgiBinPhp: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;
	ProcessorTypeEnum type;

protected:

public:
	ProcessorImplCgiBinPhp(ProcessorTypeEnum type);
	~ProcessorImplCgiBinPhp();
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual ProcessorTypeEnum getType();
	void addProperty(std::string name, std::string value);

	std::string readRequest(int clientFd);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
	std::string getBasePath();
	void setBasePath(std::string basePath);
};

