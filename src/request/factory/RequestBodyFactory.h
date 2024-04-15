#pragma once

#include "../../API/RequestHeader.h"
#include "../../config.h"
#include REQUEST_BODY_IMPL_INC

class RequestBodyFactory
{
public:
	RequestBodyFactory();
	~RequestBodyFactory();
	RequestBody* build(std::string *rawRequest, RequestHeader *reqHeader);
};

