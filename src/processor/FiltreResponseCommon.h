#pragma once

#include "Processor.h"
#include "../Harl.h"

#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/API/Response.h"
#include "../location/ProcessorAndLocationToProcessor.h"

class FiltreResponseCommon: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;
	ProcessorTypeEnum type;

public:
	FiltreResponseCommon(ProcessorTypeEnum type);
	~FiltreResponseCommon();
	virtual void setConfig(Config *conf);
	virtual ProcessorTypeEnum getType();
	virtual std::string toString();
	virtual Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void addProperty(std::string name, std::string value);
};

