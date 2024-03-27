#pragma once

#include "../../config.h"
#include REQUEST_BODY_IMPL_INC

class RequestBodyFactory
{
public:
	RequestBodyFactory();
	~RequestBodyFactory();
	RequestBodyFactory* build(std::string *rawRequest);
};

