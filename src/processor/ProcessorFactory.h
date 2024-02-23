#pragma once
#include "Processor.h"
#include "ProcessorImplDirectFs.h"

#include <string>

class ProcessorFactory
{
public:
	Processor* build(Request *request);
};

