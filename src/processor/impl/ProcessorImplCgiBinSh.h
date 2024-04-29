#pragma once

#include <map>
#include <sstream>
#include "../../Harl.h"
#include "../../API/ResponseHeader.h"
#include "../../response/factory/ResponseHeaderFactory.h"
#include "../../util/FileUtilFactory.h"
#include "../../util/StringUtil.h"
#include "../../config/Config.h"
#include "../../response/factory/ResponseFactory.h"
#include "../../location/ProcessorAndLocationToProcessor.h"
#include "../../error/HttpErrorFactory.h"
#include "../../error/HttpReturnCodeHelper.h"
#include "../../API/Processor.h"
#include "../CGI/CGIHandlerFactory.h"
#include "../../API/CGIHandler.h"
#include "../ProcessorHelper.h"

class LocationToProcessor;

class ProcessorImplCgiBinSh: public Processor
{
private:
	Harl harl;
	StringUtil stringUtil;
	Config *config;
	FileUtil fileUtil;
	ProcessorTypeEnum type;
	ProcessorHelper processorHelper;

protected:
	public:
	ProcessorImplCgiBinSh(ProcessorTypeEnum type);
	~ProcessorImplCgiBinSh();
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual void addProperty(std::string name, std::string value);
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual Config* getConfig();

	std::string readRequest(int clientFd);
	//	void closeClient(int clientFd);
	//	int getListenFd();
	//	bool isCGIRequest(const std::string &uri);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
	//	int getClientFd(int clientId);
	std::string getBasePath();
	void setBasePath(std::string basePath);
};
