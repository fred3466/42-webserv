#include "ResponseFactory.h"
#include "../../config.h"

ResponseFactory::ResponseFactory()
{
}

ResponseFactory::~ResponseFactory()
{
}
//
Response* ResponseFactory::build(ResponseHeader *header)
{
	Response *res = new RESPONSE_IMPL_CLASS(header);
	return res;
}
