#pragma once

#include "../../API/Processor.h"
#include "../../API/Request.h"
#include "../../API/Response.h"
#include "../../Harl.h"
#include "../../error/HttpErrorFactory.h"
#include "../../util/FileUtil.h"
#include <filesystem>
#include <fstream>
#include <cstdio>

class ProcessorImplDelete : public Processor
{
private:
    Harl harl;
    ProcessorTypeEnum type;
    Config *config;

public:
    ProcessorImplDelete(ProcessorTypeEnum type);
    virtual ~ProcessorImplDelete();
    virtual Response *process(Request *request, Response *response,
                              ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
    // Response *handleHttpError(int errorCode, Response *response, HttpErrorFactory &errorFactory);
    virtual void setConfig(Config *conf);
    virtual std::string toString();
    virtual void addProperty(std::string name, std::string value);
    virtual ProcessorTypeEnum getType();
    virtual bool isExclusif();
    virtual bool isBypassingExclusif();

    // Response *createErrorResponse(int errorCode);
    // Response *handleHttpError(int errorCode);
};
