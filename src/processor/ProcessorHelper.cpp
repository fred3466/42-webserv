#include "ProcessorHelper.h"

ProcessorHelper::ProcessorHelper() : errorFactory() {
}

ProcessorHelper::~ProcessorHelper() {
}

void ProcessorHelper::suppressRouteFromURI(ProcessorAndLocationToProcessor *processorAndLocationToProcessor, std::string *uriStr) {
	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	size_t ite = uriStr->find(patPath);
	if (ite == 0) {
		uriStr->erase(0, patPathLen);
	}
}

void ProcessorHelper::setScriptPath(std::string *scriptPath, Request *request, Harl harl, Processor *processor, std::string base_path, std::string uriStr) {

	(void) base_path;
	*scriptPath = "./" + uriStr;
	harl.debug("ProcessorHelper::setScriptPath: " + processor->toString() + ":\t" + request->getUri().getUri() + " -> " + *scriptPath);

}

HttpErrorFactory ProcessorHelper::getErrorFactory() {
	return errorFactory;
}

void ProcessorHelper::applyTransferEncodingOrContentLengthAndFinalize(Response *response, std::string *cgiOutput, bool bTransferEncoding) {
	int bodyLen = (*cgiOutput).length();
	if (bTransferEncoding) {
		std::string bodyLenHexaStr = StringUtil().toHexa(bodyLen);
		*cgiOutput = bodyLenHexaStr + "\r\n" + *cgiOutput + "\r\n0\r\n\r\n";
	} else {
		*cgiOutput = "\r\n" + *cgiOutput + "\r\n\r\n";
	}
	response->setBodyLength((*cgiOutput).length());
	char *bodybin = new char[(*cgiOutput).length() + 1];
	std::copy((*cgiOutput).begin(), (*cgiOutput).end(), bodybin);
	bodybin[(*cgiOutput).length()] = '\0';
	response->setBodyBin(bodybin);
	response->setCgi(true);
}
