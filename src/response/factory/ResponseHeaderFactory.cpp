#include "ResponseHeaderFactory.h"

ResponseHeaderFactory::ResponseHeaderFactory()
{
}

ResponseHeaderFactory::~ResponseHeaderFactory()
{
}
//
ResponseHeader* ResponseHeaderFactory::build()
{
	return new ResponseHttpHeader();
}
