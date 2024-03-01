#include "ResponseFactory.h"

ResponseFactory::ResponseFactory()
{
}

ResponseFactory::~ResponseFactory()
{
}
//
Response* ResponseFactory::build(ResponseHeader *header)
{
	ResponseHttp *res = new ResponseHttp(header);
	return res;
}
