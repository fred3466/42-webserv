#include "ProcessorImplCgiBinPerl.h"

ProcessorImplCgiBinPerl::ProcessorImplCgiBinPerl(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil()
{
	this->type = type;
}
ProcessorImplCgiBinPerl::~ProcessorImplCgiBinPerl()
{
}

void ProcessorImplCgiBinPerl::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiBinPerl::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	//	TODO fred post
	response->getHeader()->addField("Content-Type", "text/html; charset=UTF-8");

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	//	char *body;
	//
	//	if (isCGIRequest(request->getUri()))
	//	{
	// It's a CGI request
	CGIHandler *cgiHandler = CGIHandlerFactory().build("PERL_CGI", config);

	// Response *responseHttp;

	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	std::string uri = request->getUri().getUri();
	//	std::string proto = "http:://";
	//	size_t itePostProtocole = proto.length();
	size_t ite = uri.find(patPath);
	if (ite == 0)
	{
		uri.erase(0, patPathLen);
	}

	std::string scriptPath = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uri;
	harl.debug(toString() + ":\t" + request->getUri().getUri() + " -> " + scriptPath);

	std::string interpreterPath = config->getParamStr("perl_exe", "");
	std::string
	cgiOutput = cgiHandler->executeCGIScript(interpreterPath, scriptPath, request, response);
	int bodyLen = cgiOutput.length();

	bool bTransferEncoding = true; //"" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
	if (bTransferEncoding)
	{
		std::string bodyLenHexaStr = stringUtil.toHexa(bodyLen);
		cgiOutput = bodyLenHexaStr + "\r\n" + cgiOutput + "\r\n0\r\n\r\n";
	} else
	{
		cgiOutput = "\r\n" + cgiOutput + "\r\n\r\n";
	}
	bodyLen = cgiOutput.length();

//	bodyLen += bodyLenHexaStr.length();
	response->setBodyLength(bodyLen);

	char *bodybin = new char[cgiOutput.length()];
	std::copy(cgiOutput.begin(), cgiOutput.end(), bodybin);
	bodybin[cgiOutput.length()] = '\0';

	// char *bodybin = new char[cgiOutput.length()];
	// memcpy(bodybin, cgiOutput.data(), cgiOutput.length());
	//	bodybin = const_cast<char*>(cgiOutput.data());

	response->setBodyBin(bodybin);
	// Generate HTTP response from CGI output
	//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	//	response->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");

	response->setCgi(true);
	//	response->setHttpError(HttpErrorFactory.build(200));
	//
	//
	//	Dans filtre:
	//	response->getHttpError()

	//		resp->getHeader()->addField("\r\n");

	//	std::string httpResponse = generateHttpResponse(cgiOutput);
	// Send HTTP response back to the client
	//		sendResponse(e.getFdClient(), httpResponse);
	//	}
	//
	return response;
}

std::string ProcessorImplCgiBinPerl::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinPerl::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinPerl::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinPerl::toString()
{
	return "ProcessorImplCgiBinPerl";
}

ProcessorTypeEnum ProcessorImplCgiBinPerl::getType()
{
	return type;
}
