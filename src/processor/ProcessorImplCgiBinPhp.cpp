#include "ProcessorImplCgiBinPhp.h"

ProcessorImplCgiBinPhp::ProcessorImplCgiBinPhp(ProcessorTypeEnum type) : harl(), stringUtil(), config()
{
	this->type = type;
}
ProcessorImplCgiBinPhp::~ProcessorImplCgiBinPhp()
{
}

void ProcessorImplCgiBinPhp::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiBinPhp::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);
	FileUtil *fu = FileUtilFactory().build();

	//	std::string path = "C:\\Users\\Sauleyayan\\Desktop\\New folder";

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
//	char *body;
//
//	if (isCGIRequest(request->getUri()))
//	{
	// It's a CGI request
	CGIHandler cgiHandler;

	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	std::string uri = request->getUri();
//	std::string proto = "http:://";
//	size_t itePostProtocole = proto.length();
	size_t ite = uri.find(patPath);
	if (ite == 0)
	{
		uri.erase(0, patPathLen);
	}

	// Determine script path from the URI
	std::string scriptPath = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uri;
	harl.debug(toString() + ":\t" + request->getUri() + " -> " + scriptPath);

	// Execute the CGI script and get output
	std::string interpreterPath = config->getParamStr("php_exe", "");
	std::string
	cgiOutput = cgiHandler.executeCGIScript(interpreterPath, scriptPath, request, response);
	resp->setBodyLength(cgiOutput.length());
	char *bodybin = new char[cgiOutput.length()];
	memcpy(bodybin, cgiOutput.data(), cgiOutput.length());
//	bodybin = const_cast<char*>(cgiOutput.data());
	resp->setBodyBin(bodybin);
	// Generate HTTP response from CGI output
	//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	resp->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
//		resp->getHeader()->addField("\r\n");

//	std::string httpResponse = generateHttpResponse(cgiOutput);
	// Send HTTP response back to the client
//		sendResponse(e.getFdClient(), httpResponse);
//	}
//
	return resp;
}

std::string ProcessorImplCgiBinPhp::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinPhp::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinPhp::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinPhp::toString()
{
	return "ProcessorImplCgiBinPhp";
}

ProcessorTypeEnum ProcessorImplCgiBinPhp::getType()
{
	return type;
}
