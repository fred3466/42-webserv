#pragma once
#include "Processor.h"
#include "ProcessorImplDirectFs.h"

class ProcessorImplDirectFs: public Processor
{
public:
	Response* process(Request *request);
};

