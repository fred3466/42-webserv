#pragma once

#include "../processor/Processor.h"
#include "LocationToProcessor.h"

class Processor;

class ProcessorAndLocationToProcessor
{
private:
	Processor *processor;
	LocationToProcessor *locationToProcessor;

public:
	ProcessorAndLocationToProcessor(Processor *proc, LocationToProcessor *lp);
	~ProcessorAndLocationToProcessor();
	LocationToProcessor* getLocationToProcessor();
	Processor* getProcessor();
};

