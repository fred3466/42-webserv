#pragma once
#include "Request.h"
#include <string>
//#include "RequestHttp.h"

class RequestFactory
{
private:
//	RequestFactory(const RequestFactory &other);
//	RequestFactory& operator=(const RequestFactory &other);

public:
	RequestFactory();
	~RequestFactory();
	Request* build(std::string *rawContent);
};

