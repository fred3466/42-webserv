#include "ProcessorHelper.h"

ProcessorHelper::ProcessorHelper()
{
}

ProcessorHelper::~ProcessorHelper()
{
}

void ProcessorHelper::suppressRouteFromURI(ProcessorAndLocationToProcessor *processorAndLocationToProcessor, std::string *uriStr)
{
	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	//	std::string proto = "http:://";
	//	size_t itePostProtocole = proto.length();
	size_t ite = uriStr->find(patPath);
	if (ite == 0)
	{
		uriStr->erase(0, patPathLen);
	}
}

void ProcessorHelper::setInterpreterAndScriptPath(std::string *interpreterPath, std::string *scriptPath, Request *request, Config *config, Harl harl, Processor *processor, std::string base_path,
		std::string uriStr)
{

	*scriptPath = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uriStr;
	harl.debug(processor->toString() + ":\t" + request->getUri().getUri() + " -> " + *scriptPath);
	*interpreterPath = config->getParamStr("perl_exe", "");

}

void ProcessorHelper::applyTransferEncodingOrContentLengthAndFinalize(Response *response, std::string *cgiOutput, bool bTransferEncoding)
{
	int bodyLen = (*cgiOutput).length();
	if (bTransferEncoding)
	{
		std::string bodyLenHexaStr = StringUtil().toHexa(bodyLen);
		*cgiOutput = bodyLenHexaStr + "\r\n" + *cgiOutput + "\r\n0\r\n\r\n";
	} else
	{
		*cgiOutput = "\r\n" + *cgiOutput + "\r\n\r\n";
	}
	response->setBodyLength(bodyLen);
	char *bodybin = new char[(*cgiOutput).length() + 1];
	std::copy((*cgiOutput).begin(), (*cgiOutput).end(), bodybin);
	bodybin[(*cgiOutput).length()] = '\0';
	// char *bodybin = new char[cgiOutput.length()];
	// memcpy(bodybin, cgiOutput.data(), cgiOutput.length());
	//	bodybin = const_cast<char*>(cgiOutput.data());
	response->setBodyBin(bodybin);
	// Generate HTTP response from CGI output
	//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	//	response->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
	response->setCgi(true);
}
