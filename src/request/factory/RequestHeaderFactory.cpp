#include "RequestHeaderFactory.h"
#include "../../config.h"

RequestHeaderFactory::RequestHeaderFactory()
{
}

RequestHeaderFactory::~RequestHeaderFactory()
{
}

RequestHeader* RequestHeaderFactory::build(char *rawRequest)
{
	return new REQUEST_HEADER_IMPL_CLASS(rawRequest);
}
