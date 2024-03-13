#pragma once
#include <string>

#include "Processor.h"
#include "ProcessorImplDirectFs.h"
#include "../location/ProcessorLocator.h"
#include "../util/StringUtil.h"
#include REQUEST_HEADER_IMPL_INC_PHP

class ProcessorFactory
{
private:
	ProcessorLocator *processorLocator;
	ProcessorFactory();
	public:
	~ProcessorFactory();
	ProcessorFactory(ProcessorLocator *pl);
	Processor* build(std::string procName);
	std::vector<Processor*>* build(Request *request);
};
