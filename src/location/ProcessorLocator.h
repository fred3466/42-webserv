#pragma once
#include <vector>

#include "LocationToProcessor.h"
#include "ProcessorAndLocationToProcessor.h"
#include "../Harl.h"

class ProcessorLocator
{
private:
	std::vector<LocationToProcessor*> locationToProcessorVector;
	Harl harl;

public:
	ProcessorLocator();
	~ProcessorLocator();
	std::vector<LocationToProcessor*> getLocationToProcessorVector();
	void addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor);
	std::vector<ProcessorAndLocationToProcessor*>* listOrderedProcessorForUrlAndExt(std::string urlPath,
			std::string ext);
};

