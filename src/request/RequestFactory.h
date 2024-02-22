#pragma once
#include "Request.h"
#include <string>

class RequestFactory
{
private:
//	RequestFactory(const RequestFactory &other);
//	RequestFactory& operator=(const RequestFactory &other);

public:
	RequestFactory();
	~RequestFactory();
	Request* build(std::string rawContent);
};

