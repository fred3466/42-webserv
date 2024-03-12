#pragma once
#include "../API/RequestHeader.h"
#include "../../config.h"
#include REQUEST_HEADER_IMPL_INC

class RequestHeaderFactory
{
public:
	RequestHeaderFactory();
	~RequestHeaderFactory();
	RequestHeader* build(std::string *rawRequest);
};

