#pragma once
#include <string>

#include "../config.h"
#include "Processor.h"
#include "../location/ProcessorLocator.h"
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../util/StringUtil.h"
#include REQUEST_HEADER_IMPL_INC_PHP
#include REQUEST_HEADER_IMPL_INC_STATIC
#include REQUEST_HEADER_IMPL_INC_MIMETYPE
#include REQUEST_HEADER_IMPL_INC_COMMON
class ProcessorFactory
{
private:
	ProcessorLocator *processorLocator;
	ProcessorFactory();
	public:
	~ProcessorFactory();
	ProcessorFactory(ProcessorLocator *pl);
	Processor* build(std::string procName);
	std::vector<ProcessorAndLocationToProcessor*>* build(Request *request);
};
