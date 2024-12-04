#pragma once

#include <map>
#include <sstream>

#include "../../Harl.h"
#include "../../API/ResponseHeader.h"
#include "../../util/FileUtilFactory.h"
#include "../../util/StringUtil.h"
#include "../../config/Config.h"
#include "../../location/ProcessorAndLocationToProcessor.h"
#include "../../API/Processor.h"

#include "../../response/factory/ResponseFactory.h"
#include "../../response/factory/ResponseHeaderFactory.h"
#include "../../error/HttpErrorFactory.h"
#include "../../error/HttpReturnCodeHelper.h"
#include "../CGI/CGIHandlerFactory.h"
#include "../ProcessorHelper.h"

class LocationToProcessor;

class ProcessorImplCgiBinGeneric: public Processor {
private:
	Harl harl;
	StringUtil stringUtil;
	Config *config;
	FileUtil fileUtil;
	ProcessorTypeEnum type;
	ProcessorHelper processorHelper;

	void applyTransferEncodingOrContentLengthAndFinalize(Response *response, std::string cgiOutput);

protected:
	public:
	ProcessorImplCgiBinGeneric(ProcessorTypeEnum type);
	~ProcessorImplCgiBinGeneric();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual void addProperty(std::string name, std::string value);
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual Config* getConfig();
	virtual bool isRedirect();
	virtual bool isCgi();
	virtual bool isUriDirectoryValidationRequired();

	std::string readRequest(int clientFd);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
	std::string getBasePath();
	void setBasePath(std::string basePath);
};