#pragma once
#include <string>

#include "Processor.h"
#include "ProcessorImplDirectFs.h"

class ProcessorFactory
{
public:
	Processor* build(Request *request);
};

