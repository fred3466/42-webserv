#pragma once
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../Uri/Uri.h"

class ProcessorAndLocationToProcessor;
class Processor;

class ProcessorHelper
{
public:
	ProcessorHelper();
	~ProcessorHelper();

	void suppressRouteFromURI(ProcessorAndLocationToProcessor *processorAndLocationToProcessor, std::string *uriStr);
	void setInterpreterAndScriptPath(std::string *interpreterPath, std::string *scriptPath, Request *request, Config *config, Harl harl, Processor *processor, std::string base_path,
			std::string uriStr);
	void applyTransferEncodingOrContentLengthAndFinalize(Response *response, std::string *cgiOutput, bool bTransferEncoding);
};
