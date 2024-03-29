#include "RequestFactory.h"

RequestFactory::RequestFactory()
{
}

RequestFactory::~RequestFactory()
{
}

Request* RequestFactory::build(RequestHeader *header)
{
	Request *req = new REQUEST_IMPL_CLASS(header);
	return req;
}
