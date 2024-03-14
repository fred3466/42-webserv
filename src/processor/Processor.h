#pragma once
#include "../config/Config.h"
#include "../request/API/Request.h"
#include "../response/API/Response.h"

class Processor
{
public:
	Processor();
	virtual ~Processor();
	virtual Response* process(Request *request, Response *response) = 0;
	virtual void setConfig(Config *conf) = 0;
	virtual std::string toString()=0;
};
