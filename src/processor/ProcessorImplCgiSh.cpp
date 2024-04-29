#include "ProcessorImplCgiSh.h"

ProcessorImplCgiSh::ProcessorImplCgiSh(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil()
{
	this->type = type;
}
ProcessorImplCgiSh::~ProcessorImplCgiSh()
{
}

void ProcessorImplCgiSh::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiSh::process(Request *request, Response *response,
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
	CGIHandler cgiHandler;

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

	std::string interpreterPath = config->getParamStr("sh", "");
	std::string
	cgiOutput = cgiHandler.executeCGIScript(interpreterPath, scriptPath, request, response);

	response->setBodyLength(cgiOutput.length());
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

std::string ProcessorImplCgiSh::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/sh/");
}

void ProcessorImplCgiSh::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiSh::addProperty(std::string name, std::string value)
{
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplCgiSh::toString()
{
	return "ProcessorImplCgiSh";
}

ProcessorTypeEnum ProcessorImplCgiSh::getType()
{
	return type;
}

bool ProcessorImplCgiSh::isBypassingExclusif()
{
	return false;
}
