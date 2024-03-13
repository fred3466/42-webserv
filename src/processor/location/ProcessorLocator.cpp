#include "ProcessorLocator.h"

ProcessorLocator::ProcessorLocator()
{
}

ProcessorLocator::~ProcessorLocator()
{
}

std::vector<LocationToProcessor*> ProcessorLocator::getLocationToProcessorVector() const
{
	return locationToProcessorVector;
}

void ProcessorLocator::addLocationToProcessor(LocationToProcessor *lp)
{
	locationToProcessorVector.push_back(lp);
}

std::vector<Processor*>* ProcessorLocator::listOrderedProcessor(std::string ext)
{
	//TODO fuite mémoire : new
	std::vector<Processor*> *ret = new std::vector<Processor*>();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		std::string extension = lp->getExtension();
		if (ext == extension)
		{
			Processor *p = lp->getProcessor();
			ret->push_back(p);
		}
		return ret;
	}
}
