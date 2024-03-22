#pragma once
#include <cstring>
#include <sstream>
#include "Processor.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/factory/ResponseFactory.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../util/FileUtilFactory.h"

class FiltreError : Processor
{
private:
    ProcessorTypeEnum type;
    Config *config;

public:
    FiltreError(/* args */);
    ~FiltreError();

    FiltreError(ProcessorTypeEnum type);
    virtual Response *process(Request *request, Response *response,
                              ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
    virtual void setConfig(Config *conf);
    virtual std::string toString();
    virtual void addProperty(std::string name, std::string value);
    virtual ProcessorTypeEnum getType();
};
