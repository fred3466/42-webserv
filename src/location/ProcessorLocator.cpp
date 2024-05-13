#include "ProcessorLocator.h"

ProcessorLocator::ProcessorLocator() : harl(), locationToProcessorVector()
{
	locationToProcessorVector.reserve(1000);
}

ProcessorLocator::~ProcessorLocator()
{
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{
		LocationToProcessor *lp = *ite;
		Processor *p = lp->getProcessor();
		if (p)
		{
			delete p->getConfig();
			delete p;
			p = NULL;
			delete lp;
		}
	}
}

std::vector<LocationToProcessor*> ProcessorLocator::getLocationToProcessorVector()
{
	return locationToProcessorVector;
}

bool locationToProcessorComparator(LocationToProcessor *me, LocationToProcessor *o)
{
	std::string oPath = o->getUrlPath();
	std::string mePath = me->getUrlPath();

	int oSize = oPath.size();
	int meSize = mePath.size();

	std::string oHost = o->getHost();
	std::string meHost = me->getHost();

	std::string oExtension = o->getExtension();
	std::string meExtension = me->getExtension();

	int scoreMe = (meSize < oSize) * (2 ^ 4);
//	scoreMe &= (meHost < oHost) << 3;
//	scoreMe &= (mePath < oPath) << 2;
//	scoreMe &= (meExtension < oExtension);

	int scoreOther = (oSize < meSize) * (2 ^ 4);
//	scoreOther &= (oHost < meHost) << 3;
//	scoreOther &= (oPath < mePath) << 2;
//	scoreOther &= (oExtension < meExtension);

	bool ret = scoreMe < scoreOther;
	return ret;
}

void ProcessorLocator::addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor, std::string host)
{
	std::string procStr = processor->toString();
	harl.debug("ProcessorLocator::addLocationToProcessor (%s ; %s) => %s", urlPath.c_str(), ext.c_str(), procStr.c_str());
//	TODO new ici
	LocationToProcessor *lp = new LocationToProcessor(urlPath, ext, processor, host);

//	LocationToProcessor *existingLp = locationToProcessorVector.at(lp

	locationToProcessorVector.push_back(lp);

	std::sort(locationToProcessorVector.begin(), locationToProcessorVector.end(), &locationToProcessorComparator);
}

std::vector<ProcessorAndLocationToProcessor*>* ProcessorLocator::listOrderedProcessorForUrlAndExt(Request *request)
{
	StringUtil su = StringUtil();
	std::string pathReq = request->getPath();
	std::string extReq = su.strUpperCase(request->getFileExtension());
	std::string hostReq = request->getHost();
	int portReq = request->getPort();
	hostReq = hostReq + ":" + su.strFromInt(portReq);
//	std::string hostReq = request->getHeaderFieldValue("Host");
//TODO fuite mémoire : new
	std::vector<ProcessorAndLocationToProcessor*> *ret = new std::vector<ProcessorAndLocationToProcessor*>();
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

//		matcher le plus long urlPathIte sur l'url =>classer les motifs url par ordre décroissant de taille
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

				if ((p != NULL))
				{
					std::string msg = "ProcessorLocator::listOrderedProcessorForUrlAndExt MATCH : path [%s] ext [%s] -> ["
							+ p->toString()
							+ "]";
					ProcessorAndLocationToProcessor *processorAndLocationToProcessor = new ProcessorAndLocationToProcessor(p, lp);
					harl.debug(msg, urlPathIteChar, extensionChar);
					ret->push_back(processorAndLocationToProcessor);
				}
			}
		}
	}
	return ret;
}
