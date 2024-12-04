#pragma once

#include "../../Harl.h"
#include "../../config.h"

#include "../../util/FileUtilFactory.h"
#include "../../util/StringUtil.h"
#include "../../config/Config.h"
#include "../../API/Response.h"
#include "../../location/ProcessorAndLocationToProcessor.h"
#include "../../error/HttpErrorFactory.h"
#include "../../API/Processor.h"
#include "../ProcessorHelper.h"

class FiltreResponseRedirect: public Processor {
private:
	Harl harl;
	Config *config;
	FileUtil fileUtil;
	StringUtil stringUtil;
	ProcessorTypeEnum type;
	ProcessorHelper processorHelper;

public:
	FiltreResponseRedirect(ProcessorTypeEnum type);
	~FiltreResponseRedirect();
	virtual void setConfig(Config *conf);
	virtual ProcessorTypeEnum getType();
	virtual std::string toString();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	virtual void addProperty(std::string name, std::string value);
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual Config* getConfig();
	virtual bool isRedirect();
	virtual bool isCgi();
	virtual bool isUriDirectoryValidationRequired();
};
