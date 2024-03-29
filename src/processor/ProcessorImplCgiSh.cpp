#include "ProcessorImplCgiSh.h"

ProcessorImplCgiSh::ProcessorImplCgiSh(ProcessorTypeEnum type) : harl(), stringUtil(), config()
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

Response *ProcessorImplCgiSh::process(Request *request, Response *response,
									  ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	FileUtil *fu = FileUtilFactory().build();

	std::string base_path = config->getParamStr("base_path", "base_path_missing");

	// It's a CGI request
	CGIHandler cgiHandler;

	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	std::string uri = request->getUri();
	size_t ite = uri.find(patPath);
	if (ite == 0)
	{
		uri.erase(0, patPathLen);
	}

	// Determine script path from the URI
	std::string scriptPath = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uri;
	harl.debug(toString() + ":\t" + request->getUri() + " -> " + scriptPath);

	// Execute the CGI script and get output
	std::string interpreterPath = config->getParamStr("sh", "");
	std::string
		cgiOutput = cgiHandler.executeCGIScript(interpreterPath, scriptPath, request, response);
	response->setBodyLength(cgiOutput.length());
	char *bodybin = new char[cgiOutput.length()];
	memcpy(bodybin, cgiOutput.data(), cgiOutput.length());
	//	bodybin = const_cast<char*>(cgiOutput.data());
	response->setBodyBin(bodybin);
	// Generate HTTP response from CGI output
	//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	// response->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
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
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiSh::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiSh::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiSh::toString()
{
	return "ProcessorImplCgiSh";
}

ProcessorTypeEnum ProcessorImplCgiSh::getType()
{
	return type;
}
