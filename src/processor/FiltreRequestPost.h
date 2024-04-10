#pragma once

#include <iostream>

#include "Processor.h"
#include "../Harl.h"
#include "../config.h"

#include "../util/FileUtil.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../request/factory/RequestBodyFactory.h"
#include "../error/HttpErrorFactory.h"

class FiltreRequestPost: public Processor
{
private:
	Harl harl;
	Config *config;
	FileUtil fileUtil;
	StringUtil stringUtil;
	ProcessorTypeEnum type;

public:
	FiltreRequestPost(ProcessorTypeEnum type);
	~FiltreRequestPost();
	virtual void setConfig(Config *conf);
	virtual ProcessorTypeEnum getType();
	virtual std::string toString();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void addProperty(std::string name, std::string value);
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
};
