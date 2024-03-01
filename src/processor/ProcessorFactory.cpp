#include "ProcessorFactory.h"

Processor* ProcessorFactory::build(Request *request)
{
//	if(request.getUri()== ...
	return new ProcessorImplDirectFs();
}
