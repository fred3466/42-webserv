#pragma once
#include "../../API/ResponseHeader.h"
#include "../../API/Response.h"
#include "../../config.h"
#include RESPONSE_IMPL_INC

class ResponseFactory {
public:
	ResponseFactory();
	~ResponseFactory();
	Response* build(ResponseHeader *header);
};

