#pragma once
#include <string>

#include "../config.h"
#include "../location/ProcessorLocator.h"
#include "../location/ProcessorAndLocationToProcessor.h"
#include "../util/StringUtil.h"
#include "API/Processor.h"
#include "impl/FiltreError.h"
#include "impl/FiltreRequestPost.h"
#include "impl/FiltreResponseCommon.h"
#include "impl/FiltreResponseMimeType.h"
#include "impl/ProcessorImplCgiBinPerl.h"
#include "impl/ProcessorImplCgiBinPhp.h"
#include "impl/ProcessorImplCgiBinSh.h"
#include "impl/ProcessorImplDirectFs.h"

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
