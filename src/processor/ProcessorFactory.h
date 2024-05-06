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
#include REQUEST_HANDLER__IMPL_INC_REDIRECT
#include REQUEST_HANDLER__IMPL_INC_DOWNLOAD
#include "impl/ProcessorImplCgiBinGeneric.h"

class ProcessorFactory
{
private:
	ProcessorLocator *processorLocator;
	//	Config *config;
	ProcessorFactory();

public:
	~ProcessorFactory();
	ProcessorFactory(ProcessorLocator *pl);
	Processor* build(std::string procName, Config *config);
	std::vector<ProcessorAndLocationToProcessor*>* build(Request *request);
};
