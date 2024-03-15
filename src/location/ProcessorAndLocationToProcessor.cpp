#include "ProcessorAndLocationToProcessor.h"

LocationToProcessor* ProcessorAndLocationToProcessor::getLocationToProcessor()
{
	return locationToProcessor;
}

Processor* ProcessorAndLocationToProcessor::getProcessor()
{
	return processor;
}

ProcessorAndLocationToProcessor::~ProcessorAndLocationToProcessor()
{
}

ProcessorAndLocationToProcessor::ProcessorAndLocationToProcessor(Processor *proc, LocationToProcessor *lp)
{
	processor = proc;
	locationToProcessor = lp;
}

