#pragma once

#include "../../config.h"
#include "../../request/API/RequestHeader.h"
#include REQUEST_BODY_IMPL_INC

class RequestBodyFactory
{
public:
	RequestBodyFactory();
	~RequestBodyFactory();
	RequestBody* build(std::string *rawRequest, RequestHeader *reqHeader);
};

