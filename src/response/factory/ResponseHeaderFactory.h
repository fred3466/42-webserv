#pragma once
#include "../API/ResponseHeader.h"
#include "../../config.h"
#include RESPONSE_HEADER_IMPL_INC

class ResponseHeaderFactory
{
public:
	ResponseHeaderFactory();
	~ResponseHeaderFactory();
	ResponseHeader* build();
};

