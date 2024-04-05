#include "ProcessorImplCgiBinPhp.h"

ProcessorImplCgiBinPhp::ProcessorImplCgiBinPhp(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil()
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

// Response *ProcessorImplCgiBinPhp::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
// {
// 	if (response->getHttpError() != NULL)
// 	{
// 		// Use stringstream for number to string conversion
// 		std::stringstream ss;
// 		ss << response->getHttpError()->getCode();
// 		std::string errorCodeStr = ss.str();

// 		// Prepare an error message based on the error code and description
// 		std::string errorMessage = "Error " + errorCodeStr + ": " + response->getHttpError()->getDescription();

// 		// Allocate memory for the error message and copy it over
// 		char *errorMessageBin = new char[errorMessage.length() + 1];
// 		std::copy(errorMessage.begin(), errorMessage.end(), errorMessageBin);
// 		errorMessageBin[errorMessage.length()] = '\0'; // Null-terminate the string

// 		// Set the binary body and its length in the response object
// 		response->setBodyBin(errorMessageBin);			// Pass the allocated memory to the response
// 		response->setBodyLength(errorMessage.length()); // Set the length of the error message

// 		// Set Content-Type for the error message
// 		response->getHeader()->addField("Content-Type", "text/plain; charset=UTF-8");

// 		// You will also need to adapt the status line setting using stringstream
// 		std::stringstream statusLineSS;
// 		statusLineSS << "HTTP/1.1 " << response->getHttpError()->getCode() << " " << response->getHttpError()->getDescription() << "\r\n";
// 		std::string statusLine = statusLineSS.str();
// 		response->getHeader()->setStatusLine(statusLine);

// 		return response; // Stop further processing and return the error response
// 	}

// 	// Continue with CGI processing if no error was set
// 	response->getHeader()->addField("Content-Type", "text/html; charset=UTF-8");
// 	std::string base_path = config->getParamStr("base_path", "base_path_missing");
// 	CGIHandler cgiHandler;
// 	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
// 	std::string patPath = locationToProcessor->getUrlPath();
// 	int patPathLen = patPath.length();
// 	std::string uri = request->getUri().getUri();
// 	size_t ite = uri.find(patPath);
// 	if (ite == 0)
// 	{
// 		uri.erase(0, patPathLen);
// 	}

// 	std::string scriptPath = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uri;
// 	harl.debug(toString() + ":\t" + request->getUri().getUri() + " -> " + scriptPath);

// 	std::string interpreterPath = config->getParamStr("php_exe", "");
// 	std::string cgiOutput = cgiHandler.executeCGIScript(interpreterPath, scriptPath, request, response);

// 	response->setBodyLength(cgiOutput.length());
// 	char *bodybin = new char[cgiOutput.length()];
// 	std::copy(cgiOutput.begin(), cgiOutput.end(), bodybin);
// 	bodybin[cgiOutput.length()] = '\0';
// 	response->setBodyBin(bodybin);

// 	response->setCgi(true);

// 	return response;
// }

Response *ProcessorImplCgiBinPhp::process(Request *request, Response *response,
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

	std::string interpreterPath = config->getParamStr("php_exe", "");
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
