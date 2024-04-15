#pragma once
#include <cstring>
#include <sstream>
#include "../../response/factory/ResponseHeaderFactory.h"
#include "../../response/factory/ResponseFactory.h"
#include "../../util/FileUtilFactory.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../../API/Processor.h"
#include "../../error/HttpErrorFactory.h"
#include "../../error/HttpReturnCodeHelper.h"
#include "../ProcessorHelper.h"

class FiltreError: public Processor
{
private:
	ProcessorTypeEnum type;
	Config *config;
	ProcessorHelper processorHelper;

public:
	FiltreError(/* args */);
	~FiltreError();

	FiltreError(ProcessorTypeEnum type);
	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();

	Response* generateErrorResponse(int errorCode, const std::string &errorMessage);
	std::string loadErrorPageTemplate();
	void replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage);
};
