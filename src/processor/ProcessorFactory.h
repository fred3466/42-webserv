#pragma once
#include <string>

#include "Processor.h"
#include "ProcessorImplDirectFs.h"
#include "../util/StringUtil.h"

class ProcessorFactory
{
public:
	Processor *build(Request *request);
};
