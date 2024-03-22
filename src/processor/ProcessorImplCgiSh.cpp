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

Response* ProcessorImplCgiSh::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	FileUtil *fu = FileUtilFactory().build();

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
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
	std::string interpreterPath = config->getParamStr("Sh", "");
	std::string
	cgiOutput = cgiHandler.executeCGIScript(interpreterPath, scriptPath, request, response);
	response->setBodyLength(cgiOutput.length());
	char *bodybin = new char[cgiOutput.length()];
	memcpy(bodybin, cgiOutput.data(), cgiOutput.length());
//	bodybin = const_cast<char*>(cgiOutput.data());
	response->setBodyBin(bodybin);
	// Generate HTTP response from CGI output
	//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	response->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
	response->setCgi(true);
//		resp->getHeader()->addField("\r\n");

//	std::string httpResponse = generateHttpResponse(cgiOutput);
	// Send HTTP response back to the client
//		sendResponse(e.getFdClient(), httpResponse);
//	}
//
	return response;
}

bool ProcessorImplCgiSh::isCGIRequest(const std::string &uri)
{
	return uri.find("/cgi-bin/") == 0;
}

std::map<std::string, std::string> ProcessorImplCgiSh::prepareCGIEnvironment(Request *request)
{
	std::map<std::string, std::string> env = std::map<std::string, std::string>(); // @suppress("Invalid template argument")

	env["REQUEST_METHOD"] = request->getMethod();
	env["QUERY_STRING"] = request->getQueryString();
	env["CONTENT_TYPE"] = request->getHeaderFieldValue("Content-Type");
	env["CONTENT_LENGTH"] = request->getHeaderFieldValue("Content-Length");
	env["HTTP_COOKIE"] = request->getHeader()->getCookieString();
	env["HTTP_USER_AGENT"] = request->getHeaderFieldValue("User-Agent");
	env["PATH_INFO"] = request->getUri();
	env["REMOTE_ADDR"] = "";
	env["REMOTE_HOST"] = "";
	env["SCRIPT_FILENAME"] = getScriptPath(request->getFileName());
	env["SCRIPT_NAME"] = "";
	env["SERVER_NAME"] = "";
	env["SERVER_SOFTWARE"] = "Webserv/1.0";

	return env;
}

std::string readRequest(int clientFd)
{
	char buffer[BUF_SIZE];
	std::string requestText;
	int nbytes;

	while ((nbytes = recv(clientFd, buffer, sizeof(buffer), 0)) > 0)
	{
		requestText.append(buffer, nbytes);
	}
	if (nbytes == 0)
	{
		std::cout << "Client disconnected." << std::endl;
	}
	else if (nbytes < 0)
	{
		std::cerr << "recv() error: " << strerror(errno) << std::endl;
	}

	return requestText;
}

ProcessorTypeEnum ProcessorImplCgiSh::getType()
{
	return type;
}
