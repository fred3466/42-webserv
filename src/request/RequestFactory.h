#pragma once
#include <string>

#include "Request.h"
#include "RequestHttp.h"

class RequestFactory
{
private:

public:
	RequestFactory();
	~RequestFactory();
	Request* build(std::string *rawContent);
};

