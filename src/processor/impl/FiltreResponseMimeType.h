#pragma once
#include "../../mimeType/MimeTypeHelper.h"
#include "../../request/RequestHttp.h"
#include "../../response/ResponseHttp.h"
#include "../../response/factory/ResponseHeaderFactory.h"
#include "../../response/factory/ResponseFactory.h"
#include <sstream>
#include <map>
#include "../../API/Processor.h"
#include "../ProcessorHelper.h"

class FiltreResponseMimeType: public Processor {
private:
	static MimeTypeHelper mimeTypeHelper;
	Config *config;
	ProcessorTypeEnum type;
	ProcessorHelper processorHelper;

public:
	FiltreResponseMimeType(ProcessorTypeEnum type);
	virtual ~FiltreResponseMimeType();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual Config* getConfig();
	virtual bool isRedirect();
	virtual bool isUriDirectoryValidationRequired();
	virtual bool isCgi();
};
