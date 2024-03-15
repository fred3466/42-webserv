#pragma once
#include "../config/Config.h"
#include "../request/API/Request.h"
#include "../response/API/Response.h"

class ProcessorAndLocationToProcessor;

class Processor
{
public:
	Processor();
	virtual ~Processor();
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor) = 0;
	virtual void setConfig(Config *conf) = 0;
	virtual std::string toString()=0;
	virtual void addProperty(std::string name, std::string value)=0;
};
