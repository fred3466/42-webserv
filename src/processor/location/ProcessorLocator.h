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
	std::vector<LocationToProcessor*> getLocationToProcessorVector() const;
	void addLocationToProcessor(LocationToProcessor *lp);
	std::vector<Processor*>* listOrderedProcessor(std::string ext);
};

