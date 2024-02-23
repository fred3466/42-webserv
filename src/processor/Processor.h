#pragma once
#include "../response/Response.h"
#include "../request/Request.h"

class Processor
{
public:
	~Processor();
	virtual Response* process(Request *request)=0;
};

