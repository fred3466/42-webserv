#pragma once
#include <string>

#include "../../config.h"
#include "../API/RequestHeader.h"
#include "../API/Request.h"
#include "../RequestHttp.h"

class RequestFactory
{
private:

public:
	RequestFactory();
	~RequestFactory();
	Request* build(RequestHeader *header);
};

