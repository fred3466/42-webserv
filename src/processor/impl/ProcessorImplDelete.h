#pragma once

#include "../../API/Processor.h"
#include "../../API/Request.h"
#include "../../API/Response.h"
#include "../../Harl.h"
#include "../../error/HttpErrorFactory.h"
#include "../../util/FileUtil.h"
#include <filesystem>
#include <fstream>
#include <cstdio>

class ProcessorImplDelete: public Processor {
private:
	Harl harl;
	ProcessorTypeEnum type;
	Config *config;

public:
	ProcessorImplDelete(ProcessorTypeEnum type);
	virtual ~ProcessorImplDelete();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual Config* getConfig();
	virtual bool isRedirect();
	virtual bool isCgi();
	virtual bool isUriDirectoryValidationRequired();
};
