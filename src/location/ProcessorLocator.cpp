#include "ProcessorLocator.h"

ProcessorLocator::ProcessorLocator() : locationToProcessorVector()
{
}

ProcessorLocator::~ProcessorLocator()
{
}

std::vector<LocationToProcessor*> ProcessorLocator::getLocationToProcessorVector()
{
	return locationToProcessorVector;
}

void ProcessorLocator::addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor)
{
//	TODO new ici
	LocationToProcessor *lp = new LocationToProcessor(urlPath, ext, processor);
	locationToProcessorVector.push_back(lp);
}

std::vector<Processor*>* ProcessorLocator::listOrderedProcessorForUrlAndExt(std::string urlPath, std::string ext)
{
	//TODO fuite mémoire : new
	std::vector<Processor*> *ret = new std::vector<Processor*>();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		std::string extension = lp->getExtension();
		std::string urlPathIte = lp->getUrlPath();
//		TODO : matcher le plus long urlPathIte sur l'url
		if ("." == extension || (ext == extension && urlPath == urlPathIte))
		{
			Processor *p = lp->getProcessor();
			ret->push_back(p);
		}
	}
	return ret;
}
