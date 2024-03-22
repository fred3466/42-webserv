#pragma once
#include "../config/Config.h"
#include "../request/API/Request.h"
#include "../response/API/Response.h"
#include <iterator>

class ProcessorAndLocationToProcessor;

enum ProcessorTypeEnum
{
	HEADER_MODIFIER, CONTENT_MODIFIER
};
class Processor
{
private:
	ProcessorTypeEnum type;

protected:
	Processor();

public:
	Processor(ProcessorTypeEnum type);
	virtual ~Processor();
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor) = 0;
	virtual void setConfig(Config *conf) = 0;
	virtual std::string toString()=0;
	virtual void addProperty(std::string name, std::string value)=0;
	virtual ProcessorTypeEnum getType()=0;
};
