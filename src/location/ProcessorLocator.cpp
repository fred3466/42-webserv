#include "ProcessorLocator.h"

std::vector<LocationToProcessor*> ProcessorLocator::locationToProcessorVector;

ProcessorLocator::ProcessorLocator() : harl() {
//	locationToProcessorVector.reserve(100);
}

ProcessorLocator::~ProcessorLocator() {
	Harl().info("ProcessorLocator::~ProcessorLocator : locationToProcessorVector : %i", locationToProcessorVector.size());
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin(); ite != locationToProcessorVector.end(); ite++) {
		LocationToProcessor *lp = *ite;
		if (lp) {
			delete lp;
			lp = NULL;
		}
	}
}

std::vector<LocationToProcessor*> ProcessorLocator::getLocationToProcessorVector() {
	return locationToProcessorVector;
}

bool locationToProcessorComparator(LocationToProcessor *me, LocationToProcessor *o) {
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

void ProcessorLocator::addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor, std::string host, std::string hostAsIpPort, std::string method) {
	std::string procStr = processor->toString();
	harl.debug("ProcessorLocator::addLocationToProcessor (%s ; %s) => %s", urlPath.c_str(), ext.c_str(), procStr.c_str());

	LocationToProcessor *lp = new LocationToProcessor(urlPath, ext, processor, host, hostAsIpPort, method);

	locationToProcessorVector.push_back(lp);

	std::sort(locationToProcessorVector.begin(), locationToProcessorVector.end(), &locationToProcessorComparator);
}

std::vector<ProcessorAndLocationToProcessor*>* ProcessorLocator::listOrderedProcessorForUrlAndExt(Request *request) {
	StringUtil su = StringUtil();
	std::string pathReq = request->getPath();
	std::string extReq = su.strUpperCase(request->getFileExtension());
	std::string hostPortReq = request->getHost();
	int portReq = request->getPort();
	hostPortReq = hostPortReq + ":" + su.strFromInt(portReq);
	std::string keyHost = "";
	std::string keyHostAsIpPort = "";

	std::vector<ProcessorAndLocationToProcessor*> vectorServerRouteProcessorOrderedByRouteDesc = std::vector<ProcessorAndLocationToProcessor*>();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin(); ite != locationToProcessorVector.end(); ite++) {

		LocationToProcessor *lp = *ite;
		std::string extension = lp->getExtension();
		std::string urlPath = lp->getUrlPath();
		std::string hostPort = lp->getHost();
		std::string hostAsIpPort = lp->getHostAsIpPort();
		std::string method = lp->getMethod();

		bool bHostOrIpDontMatchRequest = (hostPortReq != "" || hostPort != "") && (hostPort != hostPortReq) && (hostAsIpPort != hostPortReq);
		if (bHostOrIpDontMatchRequest) {
			continue;
		}

//		Init 1° itéaration et changement d'IP:port => nouveau 1° serveur
		if (keyHost == "" || keyHostAsIpPort != hostAsIpPort) {
			keyHost = hostPort;
			keyHostAsIpPort = hostAsIpPort;
		}

//		match sur ip:port : est-ce bien le premier host?
		if (hostPortReq == hostAsIpPort && hostAsIpPort != keyHostAsIpPort) {
			continue;
		}

//		matcher le plus long urlPathIte sur l'url =>classer les motifs url par ordre décroissant de taille
		int pos = 0;
		int len = urlPath.length();
		int nbCharMatching = pathReq.compare(pos, len, urlPath);
		bool bUrlPatternMatchEntirely = (nbCharMatching == 0);

		std::string reqMethod = request->getMethod();

		if (bUrlPatternMatchEntirely && ("." == extension || extReq == extension)) {
			Processor *p = lp->getProcessor();
			if (p) {
				const char *urlPathIteChar = urlPath.c_str();
				const char *extensionChar = extension.c_str();

				if ((p != NULL)) {
					std::string msg = "ProcessorLocator::listOrderedProcessorForUrlAndExt MATCH : path [%s] ext [%s] -> [%s]";
					ProcessorAndLocationToProcessor *processorAndLocationToProcessor = new ProcessorAndLocationToProcessor(p, lp);
					harl.debug(msg, urlPathIteChar, extensionChar, p->toString().c_str());
					vectorServerRouteProcessorOrderedByRouteDesc.push_back(processorAndLocationToProcessor);
				}
			}
		}
	} //for

	std::vector<ProcessorAndLocationToProcessor*> *retOrderedByExclusifFirst = new std::vector<ProcessorAndLocationToProcessor*>();
//	processor exclusifs en premier
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator iteRet = vectorServerRouteProcessorOrderedByRouteDesc.begin(); iteRet != vectorServerRouteProcessorOrderedByRouteDesc.end();
			iteRet++) {
		ProcessorAndLocationToProcessor *plp = *iteRet;
		Processor *proc = plp->getProcessor();
		if (proc->isExclusif()) {
			retOrderedByExclusifFirst->push_back(plp);
		}
	}
//	puis les autres
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator iteRet = vectorServerRouteProcessorOrderedByRouteDesc.begin(); iteRet != vectorServerRouteProcessorOrderedByRouteDesc.end();
			iteRet++) {
		ProcessorAndLocationToProcessor *plp = *iteRet;
		Processor *proc = plp->getProcessor();
		if (proc->isExclusif() == false) {
			retOrderedByExclusifFirst->push_back(plp);
		}

	}
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator iteRet = retOrderedByExclusifFirst->begin(); iteRet != retOrderedByExclusifFirst->end(); iteRet++) {
		ProcessorAndLocationToProcessor *plp = *iteRet;
		harl.debug("ProcessorLocator::listOrderedProcessorForUrlAndExt EXCLU FIRST : -> [%s]", plp->getProcessor()->toString().c_str());

	}

	return retOrderedByExclusifFirst;
}

bool ProcessorLocator::_checkAccess(Request *request, Response *resp, Processor *processor) {
	StringUtil su;
	std::string metReq = request->getHeader()->getMethod();
	std::string limitConfig = su.strUpperCase(processor->getConfig()->getParamStr("limit_except", ""));
	std::vector<std::string> limit_exceptTab = su.tokenize(limitConfig, ' ');
	for (int i = 0; i < (int) limit_exceptTab.size(); i++) {
		std::string methConfig = limit_exceptTab[i];
		if ((methConfig == metReq) /*|| (metReq == "HEAD" && methConfig == "GET")*/) {
			return true;
		}
	}
	harl.debug("ProcessorLocator::_checkAccess: [%s]la méthode de la requête [%s] n'est pas autorisée : [%s]", processor->toString().c_str(), metReq.c_str(), limitConfig.c_str());

	(void) resp;
//	std::string allowedMeths = su.replace_all(limitConfig, " ", ", ");
//	resp->getHeader()->addField("Allow", allowedMeths);

	return false;
}
