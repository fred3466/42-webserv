#pragma once
#include <string>

#include "../../API/Request.h"
#include "../../API/RequestHeader.h"
#include "../../config.h"
#include "../RequestHttp.h"

class RequestFactory
{
private:

public:
	RequestFactory();
	~RequestFactory();
	Request* build(RequestHeader *header, RequestBody *body);
};

