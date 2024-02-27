#pragma once
#include "../response/Response.h"
#include "../request/Request.h"
#include "../config/Config.h"

class Processor
{
public:
	Processor();
	virtual ~Processor();
	virtual Response* process(Request *request)=0;
	virtual void setConfig(Config *conf)=0;
};

