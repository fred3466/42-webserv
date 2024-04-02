#pragma once
#include <cstring>
#include <sstream>
#include "Processor.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/factory/ResponseFactory.h"
#include "../util/FileUtilFactory.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "../error/HttpErrorFactory.h"

class FiltreError: public Processor
{
private:
	ProcessorTypeEnum type;
	Config *config;

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
	Response* generateErrorResponse(int errorCode, const std::string &errorMessage);
	std::string loadErrorPageTemplate();
	void replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage);
};
