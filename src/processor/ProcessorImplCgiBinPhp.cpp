#include "ProcessorImplCgiBinPhp.h"
ProcessorImplCgiBinPhp::ProcessorImplCgiBinPhp() : harl(), stringUtil(), config()
{
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
	char *body;
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

//bool ProcessorImplCgiBinPhp::isCGIRequest(const std::string &uri)
//{
//	// Check if URI starts with /cgi-bin/
//	return uri.find("/cgi-bin/") == 0;
//}

std::string ProcessorImplCgiBinPhp::readRequest(int clientFd)
{
	char buffer[BUF_SIZE];
	std::string requestText;
	int nbytes;

	while ((nbytes = recv(clientFd, buffer, sizeof(buffer), 0)) > 0)
	{
		requestText.append(buffer, nbytes);
	}

	// Check for socket closed or error
	if (nbytes == 0)
	{
		// Connection closed
		std::cout << "Client disconnected." << std::endl;
	}
	else if (nbytes < 0)
	{
		// Error occurred
		std::cerr << "recv() error: " << strerror(errno) << std::endl;
	}

	return requestText;
}

//void ProcessorImplCgiBinPhp::closeClient(int clientFd)
//{
//	shutFd(clientFd);
//}

//int ProcessorImplCgiBinPhp::getListenFd()
//{
//	TcpConnector *tcpConnector = dynamic_cast<TcpConnector*>(connector);
//	if (tcpConnector)
//	{
//		return tcpConnector->getListenFd();
//	}
//	else
//	{
//		std::cerr << "Connector is not properly initialized or wrong type."
//				<< std::endl;
//		return -1;
//	}
//}

std::string ProcessorImplCgiBinPhp::getScriptPath(const std::string &uri)
{
	// adapt for correct path !
	// std::string basePath = "/home/parallels/Desktop/Parallels Shared Folders/42/webserv/src";
	std::string basePath = "cgi-bin/";

	return basePath + uri;
}

std::string ProcessorImplCgiBinPhp::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinPhp::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

std::string ProcessorImplCgiBinPhp::generateHttpResponse(const std::string &cgiOutput)
{
	std::string response;

	// Parse CGI output for headers and body
	size_t pos = cgiOutput.find("\r\n\r\n");
	std::string headers = cgiOutput.substr(0, pos);
	std::string body = cgiOutput.substr(pos + 4);

	// Construct HTTP response
	response = "HTTP/1.1 200 OK\r\n" + headers + "\r\n\r\n" + body;
	return response;
}

void ProcessorImplCgiBinPhp::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinPhp::toString()
{
	return "ProcessorImplCgiBinPhp";
}
//int ProcessorImplCgiBinPhp::getClientFd(int clientId)
//{
//	std::map<int, int>::const_iterator it = _clients.find(clientId);
//	if (it != _clients.end())
//	{
//		return it->second; // Return the file descriptor for the client
//	}
//	else
//	{
//		return -1; // Indicate that the client was not found
//	}
//}
