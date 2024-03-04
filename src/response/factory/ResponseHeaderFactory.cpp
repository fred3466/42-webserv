#include "ResponseHeaderFactory.h"
#include "../../config.h"

ResponseHeaderFactory::ResponseHeaderFactory()
{
}

ResponseHeaderFactory::~ResponseHeaderFactory()
{
}
//
ResponseHeader* ResponseHeaderFactory::build()
{
	return new RESPONSE_HEADER_IMPL_CLASS();
}
