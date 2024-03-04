#pragma once
#include "../config/Config.h"
#include "../request/Request.h"
#include "../response/API/Response.h"

class Processor
{
public:
	Processor();
	virtual ~Processor();
	virtual Response *process(Request *request) = 0;
	virtual void setConfig(Config *conf) = 0;
};
