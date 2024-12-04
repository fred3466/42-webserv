#pragma once
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../Uri/Uri.h"
#include "../util/FileUtil.h"
#include "../error/HttpErrorFactory.h"

class ProcessorAndLocationToProcessor;
class Processor;

class ProcessorHelper {
private:
	HttpErrorFactory errorFactory;

public:
	ProcessorHelper();
	~ProcessorHelper();

	void suppressRouteFromURI(ProcessorAndLocationToProcessor *processorAndLocationToProcessor, std::string *uriStr);
	void setScriptPath(std::string *scriptPath, Request *request, Harl harl, Processor *processor, std::string base_path, std::string uriStr);
	void applyTransferEncodingOrContentLengthAndFinalize(Response *response, std::string *cgiOutput, bool bTransferEncoding);
	HttpErrorFactory getErrorFactory();
};

