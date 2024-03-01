#include "RequestFactory.h"

RequestFactory::RequestFactory()
{
}

RequestFactory::~RequestFactory()
{
}

Request* RequestFactory::build(std::string *rawContent)
{
	RequestHttp *req = new RequestHttp(rawContent);
	return req;
}
