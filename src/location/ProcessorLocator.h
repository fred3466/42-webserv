#pragma once
#include <vector>

#include "LocationToProcessor.h"

class ProcessorLocator
{
private:
	std::vector<LocationToProcessor*> locationToProcessorVector;

public:
	ProcessorLocator();
	~ProcessorLocator();
	std::vector<LocationToProcessor*> getLocationToProcessorVector();
	void addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor);
	std::vector<Processor*>* listOrderedProcessorForUrlAndExt(std::string urlPath, std::string ext);
};

