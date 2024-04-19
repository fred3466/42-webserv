#include "ProcessorImplCgiBinSh.h"

ProcessorImplCgiBinSh::ProcessorImplCgiBinSh(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper()
{
	this->type = type;
}
ProcessorImplCgiBinSh::~ProcessorImplCgiBinSh()
{
}

void ProcessorImplCgiBinSh::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiBinSh::process(Request *request, Response *response,
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
	CGIHandler *cgiHandler = CGIHandlerFactory().build("SH_CGI", config);

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

	std::string interpreterPath = config->getParamStr("sh_exe", "");
	std::string
	cgiOutput = cgiHandler->executeCGIScript(interpreterPath, scriptPath, request, response);

	int bodyLen = cgiOutput.length();
	std::string bodyLenHexaStr = stringUtil.toHexa(bodyLen);
//	bodyLenHexaStr = stringUtil.toHexa(bodyLen + bodyLenHexaStr.length() + 2 + 7);

	cgiOutput = bodyLenHexaStr + "\r\n" + cgiOutput + "\r\n0\r\n\r\n";
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

std::string ProcessorImplCgiBinSh::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinSh::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinSh::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinSh::toString()
{
	return "ProcessorImplCgiBinSh";
}

ProcessorTypeEnum ProcessorImplCgiBinSh::getType()
{
	return type;
}

bool ProcessorImplCgiBinSh::isExclusif()
{
	return true;
}

bool ProcessorImplCgiBinSh::isBypassingExclusif()
{
	return false;
}