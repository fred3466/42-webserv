#pragma once
//#include "../ResponseHttp.h"
#include "../API/ResponseHeader.h"
#include "../API/Response.h"
#include "../../config.h"
#include RESPONSE_IMPL_INC

class ResponseFactory
{
public:
	ResponseFactory();
	~ResponseFactory();
	//	ResponseFactory(const ResponseFactory &other);
//	ResponseFactory& operator=(const ResponseFactory &other);
	Response* build(ResponseHeader *header);
};

