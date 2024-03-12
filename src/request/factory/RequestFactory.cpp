#include "RequestFactory.h"
#include "../../config.h"

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