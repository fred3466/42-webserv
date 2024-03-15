#include "ProcessorLocator.h"

ProcessorLocator::ProcessorLocator() : harl(), locationToProcessorVector()
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
	harl.debug("ROUTE : %s", lp->toString().c_str());
	locationToProcessorVector.push_back(lp);
}

std::vector<ProcessorAndLocationToProcessor*>* ProcessorLocator::listOrderedProcessorForUrlAndExt(std::string urlPath,
		std::string ext)
{
	//TODO fuite mémoire : new
	std::vector<ProcessorAndLocationToProcessor*> *ret = new std::vector<ProcessorAndLocationToProcessor*>();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		std::string extension = lp->getExtension();
		std::string urlPathIte = lp->getUrlPath();
//		TODO : matcher le plus long urlPathIte sur l'url
		int pos = 0;
		int len = urlPathIte.length();
		int nbCharMatching = urlPath.compare(pos, len, urlPathIte);
		bool bUrlPatternMatchEntirely = (nbCharMatching == 0);

		if (bUrlPatternMatchEntirely
				&& ("." == extension || ext == extension))
		{
			Processor *p = lp->getProcessor();
			if (p)
			{
				const char *urlPathIteChar = urlPathIte.c_str();
				const char *extensionChar = extension.c_str();
				const char *pChar = "NULL";

				if ((p != NULL))
					pChar = p->toString().c_str();
				std::string msg = "ProcessorLocator::listOrderedProcessorForUrlAndExt MATCH : path [%s] ext [%s] -> ["
						+ p->toString()
						+ "]";
				ProcessorAndLocationToProcessor *processorAndLocationToProcessor = new ProcessorAndLocationToProcessor(
						p, lp);
				harl.debug(msg, urlPathIteChar, extensionChar);
				ret->push_back(processorAndLocationToProcessor);
			}
		}
	}
	return ret;
}
