/*
 * ProcessorFactory.cpp
 *
 *  Created on: 22 févr. 2024
 *      Author: fbourgue
 */

#include "ProcessorFactory.h"

Processor* ProcessorFactory::build(Request *request)
{
//	if(request.getUri()== ...
	return new ProcessorImplDirectFs();
}
