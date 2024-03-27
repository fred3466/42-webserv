#include "RequestBodyFactory.h"
#include "../../config.h"

RequestBodyFactory::RequestBodyFactory()
{
}

RequestBodyFactory::~RequestBodyFactory()
{
}
RequestBodyFactory* RequestBodyFactoryFactory::build(std::string *rawRequest)
{
	return new REQUEST_BODY_IMPL_CLASS(rawRequest);
}
