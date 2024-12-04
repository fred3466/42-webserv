#pragma once
#include "../config/Config.h"
#include "Request.h"
#include "Response.h"

class ProcessorAndLocationToProcessor;
enum ProcessorTypeEnum {
	HEADER_MODIFIER, CONTENT_MODIFIER
};
class Processor {
private:
	ProcessorTypeEnum type;

protected:
	Processor();

public:
	Processor(ProcessorTypeEnum type);
	virtual ~Processor();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) = 0;
	virtual void setConfig(Config *conf) = 0;
	virtual Config* getConfig() = 0;
	virtual std::string toString() = 0;
	virtual void addProperty(std::string name, std::string value) = 0;
	virtual ProcessorTypeEnum getType() = 0;
	virtual bool isExclusif() = 0;
	virtual bool isBypassingExclusif() = 0;
	virtual bool isRedirect() = 0;
	virtual bool isCgi() = 0;
	virtual std::string getProperty(std::string name, std::string defaultVal) = 0;
	virtual bool isUriDirectoryValidationRequired()=0;
};
