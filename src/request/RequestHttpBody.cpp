#include "RequestHttpBody.h"

RequestHttpBody::RequestHttpBody(std::string *rawBody) : rawBody(rawBody)
{
}

RequestHttpBody::~RequestHttpBody()
{
}

RequestHttpBody::RequestHttpBody(const RequestHttpBody &o) : RequestBody(), rawBody(o.rawBody)
{
	if (this != &o)
		*this = o;
}

RequestHttpBody& RequestHttpBody::operator=(const RequestHttpBody &o)
{
	this->rawBody = o.rawBody;
	return *this;
}

