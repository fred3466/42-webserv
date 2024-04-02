#pragma once

#include <string>
#include <sstream>
#include "API/RequestBody.h"

class RequestHttpBody: public RequestBody
{
private:
	std::string *content;

public:
	RequestHttpBody(std::string *rawRequest);
	virtual ~RequestHttpBody();
	RequestHttpBody(const RequestHttpBody &other);
	RequestHttpBody& operator=(const RequestHttpBody &other);

	virtual std::string* getContent();
};

