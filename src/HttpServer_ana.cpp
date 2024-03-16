#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>

HttpServer::HttpServer() : harl(), connector(), config()
{
}

HttpServer::~HttpServer()
{
}

// bool HttpServer::operator==(const ConnectorListener &other)
//{
//	return *((ConnectorListener) this) == other;
// }

// bool HttpServer::operator==(const ConnectorListener &o)
//{
////	if (o)
//	return this-> == o._soListen;
////	return 0;
//}
void shutFd(int fd)
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}
void HttpServer::init(std::string ipStr, int port)
{
	netStruct ns;

	config = ConfigFactory().build();

	//	config.read("config.properties");

	connector = ConnectorFactory().build(ipStr, port);
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::onIncomming(ConnectorEvent e)
{
}

void HttpServer::onDataReceiving(ConnectorEvent e)
{
	// Retrieve the request from the event
	std::string rawRequest = e.getTemp();
	HttpRequest httpRequest(rawRequest);

	// Extract the request method and query string
	std::string requestMethod = httpRequest.getMethod();
	std::string uri = httpRequest.getUri();
	std::string queryString = ""; // Initialize queryString to an empty string
	size_t queryPos = uri.find('?');
	if (queryPos != std::string::npos)
	{
		queryString = uri.substr(queryPos + 1);
	}

	if (isCGIRequest(httpRequest.getUri()))
	{
		// It's a CGI request
		CGIHandler cgiHandler;

		// Prepare CGI environment variables
		std::map<std::string, std::string> envVars = prepareCGIEnvironment(httpRequest);

		// Determine script path from the URI
		std::string scriptPath = getScriptPath(httpRequest.getUri());

		// Execute the CGI script and get output
		std::string cgiOutput = cgiHandler.executeCGIScript(scriptPath, envVars, requestMethod, queryString);

		// Generate HTTP response from CGI output
		std::string httpResponse = generateHttpResponse(cgiOutput);

		// Send HTTP response back to the client
		sendResponse(e.getFdClient(), httpResponse);
	}
}

// void HttpServer::onDataReceiving(ConnectorEvent e)
// {
// 	//	std::cout << e.getTemp();
// 	std::string rawRequest = e.getTemp();
// 	Request *request = RequestFactory().build(&rawRequest);
// 	request->setFdClient(e.getFdClient());
// 	//	req->dump();

// 	//	Validator *validator = ValidatorFactory().build(req);
// 	//	validator->validate(req);

// 	Processor *processor = ProcessorFactory().build(request);
// 	processor->setConfig(config);
// 	Response *resp = processor->process(request);

// 	StringUtil stringUtil;
// 	std::string fieldsString = stringUtil.fromListToString(
// 		resp->getHeader()->getFields());
// 	std::string statusLine = resp->getHeader()->getStatusLine();

// 	// Send Response
// 	std::string statusHeaderBody = statusLine + fieldsString + std::string(resp->getBody());

// 	int statusLen = statusLine.length();
// 	int headerLen = fieldsString.length();
// 	int bodyLen = resp->getBodyLength();

// 	int length = statusLen + headerLen + bodyLen;

// 	if (length <= 0)
// 	{
// 		delete request;
// 		delete processor;
// 		delete resp->getHeader();
// 		delete resp->getBodyBin();
// 		delete resp;
// 		return;
// 	}

// 	char *cstr = new char[length + 0]();
// 	char *cstrSave = cstr;
// 	char **cstrPtr = &cstr;

// 	//	std::strcpy(cstr, statusHeaderBody.c_str());

// 	//	std::memcpy(cstr, statusHeaderBody.c_str(), length);

// 	std::memcpy(*cstrPtr, statusLine.c_str(), statusLen);
// 	*cstrPtr += statusLen + 0;
// 	std::memcpy(*(cstrPtr), fieldsString.c_str(), headerLen);
// 	*cstrPtr += headerLen + 0;
// 	std::memcpy(*(cstrPtr), resp->getBodyBin(), bodyLen);

// 	cstr = cstrSave;

// 	//	if (bodyLen)
// 	//	{
// 	//		std::ofstream os("out2.html", std::ios::binary | std::ios::out);
// 	//		os.write(cstr, length);
// 	//		os.close();
// 	//	}

// 	int fd = e.getFdClient();
// 	if (request && fd && cstr && length)
// 		send(fd, cstr, length, 0);
// 	harl.debug("%d sent %d bytes through the wire", fd, length);
// 	harl.trace("%s", cstr);

// 	delete request;
// 	delete processor;
// 	delete resp->getHeader();
// 	delete resp->getBodyBin();
// 	delete resp;

// 	//	write(request->getFdClient(), cstr, length);
// 	//	rc = send(curentPollFd.fd, buffer, len, 0);
// 	//	if (rc < 0) {
// 	//		harl.error("  send() failed");
// 	//		close_conn = 1;
// 	//		break;
// 	//	}
// }

std::string HttpServer::readRequest(int clientFd)
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

void HttpServer::sendResponse(int clientFd, const std::string &response)
{
	send(clientFd, response.c_str(), response.size(), 0);
}

void HttpServer::closeClient(int clientFd)
{
	shutFd(clientFd);
}

int HttpServer::getListenFd()
{
	TcpConnector *tcpConnector = dynamic_cast<TcpConnector*>(connector);
	if (tcpConnector)
	{
		return tcpConnector->getListenFd();
	}
	else
	{
		std::cerr << "Connector is not properly initialized or wrong type."
				<< std::endl;
		return -1;
	}
}

bool HttpServer::isCGIRequest(const std::string &uri)
{
	// Check if URI starts with /cgi-bin/
	return uri.find("/cgi-bin/") == 0;
}

std::map<std::string, std::string> HttpServer::prepareCGIEnvironment(const HttpRequest &request)
{
	size_t queryPos = request.getUri().find('?');
	// Clear existing environment variables
	env.clear();

	// Populate environment variables
	env["REQUEST_METHOD"] = request.getMethod();
	if (queryPos != std::string::npos)
	{
		env["QUERY_STRING"] = request.getUri().substr(queryPos + 1);
	}
	else
	{
		env["QUERY_STRING"] = "";
	}
	env["CONTENT_TYPE"] = request.getValue("Content-Type");
	env["CONTENT_LENGTH"] = request.getValue("Content-Length");
	return env;
}

std::string HttpServer::getScriptPath(const std::string &uri)
{
	// adapt for correct path !
	// std::string basePath = "/home/parallels/Desktop/Parallels Shared Folders/42/webserv/src";
	std::string basePath = "/tmp";

	return basePath + uri;
}

std::string HttpServer::generateHttpResponse(const std::string &cgiOutput)
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

int HttpServer::getClientFd(int clientId)
{
	std::map<int, int>::const_iterator it = _clients.find(clientId);
	if (it != _clients.end())
	{
		return it->second; // Return the file descriptor for the client
	}
	else
	{
		return -1; // Indicate that the client was not found
	}
}
