#pragma once

#include <string>
#include "API/RequestBody.h"

class RequestHttpBody: public RequestBody
{
private:
	std::string *rawBody;

public:
	RequestHttpBody(std::string *rawBody);
	~RequestHttpBody();
	RequestHttpBody(const RequestHttpBody &other);
	RequestHttpBody& operator=(const RequestHttpBody &other);
};

