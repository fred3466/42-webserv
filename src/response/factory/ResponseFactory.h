#pragma once
#include "../ResponseHttp.h"
class ResponseFactory
{
public:
	ResponseFactory();
	~ResponseFactory();
	//	ResponseFactory(const ResponseFactory &other);
//	ResponseFactory& operator=(const ResponseFactory &other);
	Response* build(ResponseHeader *header);
};

