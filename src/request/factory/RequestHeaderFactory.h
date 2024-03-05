#pragma once
#include "../API/RequestHeader.h"
#include "../../config.h"
#include RESPONSE_HEADER_IMPL_INC

class RequestHeaderFactory
{
public:
	RequestHeaderFactory();
	~RequestHeaderFactory();
	RequestHeader* build();
};

