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

void ProcessorLocator::addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor, std::string host)
{
//	TODO new ici
	LocationToProcessor *lp = new LocationToProcessor(urlPath, ext, processor, host);
	harl.debug("ROUTE : %s", lp->toString().c_str());
	locationToProcessorVector.push_back(lp);
}

std::vector<ProcessorAndLocationToProcessor*>* ProcessorLocator::listOrderedProcessorForUrlAndExt(Request *request)
{
	std::string pathReq = request->getPath();
	std::string extReq = request->getFileExtension();
//	std::string hostReq = request->getHost();
	std::string hostReq = request->getHeaderFieldValue("Host");
	//TODO fuite mémoire : new
	std::vector<ProcessorAndLocationToProcessor*>
	*ret = new std::vector<ProcessorAndLocationToProcessor*>();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		std::string extension = lp->getExtension();
		std::string urlPath = lp->getUrlPath();
		std::string host = lp->getHost();

		if ((hostReq != "" || host != "") && (host != hostReq))
		{
			continue;
		}

//		TODO : matcher le plus long urlPathIte sur l'url =>classer les motifs url par ordre décroissant de taille
		int pos = 0;
		int len = urlPath.length();
		int nbCharMatching = pathReq.compare(pos, len, urlPath);
		bool bUrlPatternMatchEntirely = (nbCharMatching == 0);

		if (bUrlPatternMatchEntirely
				&& ("." == extension || extReq == extension))
		{
			Processor *p = lp->getProcessor();
			if (p)
			{
				const char *urlPathIteChar = urlPath.c_str();
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
