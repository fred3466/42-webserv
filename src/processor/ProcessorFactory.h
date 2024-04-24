#pragma once
#include <string>

#include "../API/Processor.h"
#include "../config.h"
#include "../location/ProcessorLocator.h"
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../util/StringUtil.h"
#include REQUEST_HEADER_IMPL_INC_ERROR
#include REQUEST_HEADER_IMPL_INC_POST
#include REQUEST_HEADER_IMPL_INC_COMMON
#include REQUEST_HEADER_IMPL_INC_MIMETYPE
#include REQUEST_HANDLER_IMPL_INC_PERL
#include REQUEST_HEADER_IMPL_INC_SH
#include REQUEST_HEADER_IMPL_INC_STATIC
#include REQUEST_HEADER_IMPL_INC_DELETE

class ProcessorFactory
{
private:
	ProcessorLocator *processorLocator;
	ProcessorFactory();

public:
	~ProcessorFactory();
	ProcessorFactory(ProcessorLocator *pl);
	Processor *build(std::string procName);
	std::vector<ProcessorAndLocationToProcessor *> *build(Request *request);
};
