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

Response* ProcessorImplCgiBinPhp::process(Request *request)
{
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);
	FileUtil *fu = FileUtilFactory().build();

	//	std::string path = "C:\\Users\\Sauleyayan\\Desktop\\New folder";

	std::string root = config->getParamStr("root", "root");
	std::string path = root + request->getUri();
	harl.info(request->getUri() + " -> " + path);
	char *body;
//
	if (isCGIRequest(request->getUri()))
	{
		// It's a CGI request
		CGIHandler cgiHandler;

		// Prepare CGI environment variables
		std::map<std::string, std::string> envVars = prepareCGIEnvironment(request); // @suppress("Invalid template argument")

		// Determine script path from the URI
		std::string scriptPath = getScriptPath(request->getUri());

		// Execute the CGI script and get output
		std::string cgiOutput = cgiHandler.executeCGIScript(scriptPath, envVars, request->getMethod(), // @suppress("Invalid arguments")
				request->getQueryString());

		// Generate HTTP response from CGI output
		std::string httpResponse = generateHttpResponse(cgiOutput);

		// Send HTTP response back to the client
//		sendResponse(e.getFdClient(), httpResponse);
	}
//
	return resp;
}

bool ProcessorImplCgiBinPhp::isCGIRequest(const std::string &uri)
{
	// Check if URI starts with /cgi-bin/
	return uri.find("/cgi-bin/") == 0;
}

std::map<std::string, std::string> ProcessorImplCgiBinPhp::prepareCGIEnvironment(Request *request) // @suppress("Invalid template argument") // @suppress("Member declaration not found")
{
	std::map<std::string, std::string> env = std::map<std::string, std::string>(); // @suppress("Invalid template argument")

	// Populate environment variables
	env["REQUEST_METHOD"] = request->getMethod();
	env["QUERY_STRING"] = request->getQueryString();
	env["CONTENT_TYPE"] = request->getHeaderFieldValue("Content-Type");
	env["CONTENT_LENGTH"] = request->getHeaderFieldValue("Content-Length");

//	TODO : il manque quelques variables !
//	fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
//	fastcgi_param  QUERY_STRING       $query_string;
//	fastcgi_param  REQUEST_METHOD     $request_method;
//	fastcgi_param  CONTENT_TYPE       $content_type;
//	fastcgi_param  CONTENT_LENGTH     $content_length;
//
//	fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
//	fastcgi_param  REQUEST_URI        $request_uri;
//	fastcgi_param  DOCUMENT_URI       $document_uri;
//	fastcgi_param  DOCUMENT_ROOT      $document_root;
//	fastcgi_param  SERVER_PROTOCOL    $server_protocol;
//	fastcgi_param  REQUEST_SCHEME     $scheme;
//	fastcgi_param  HTTPS              $https if_not_empty;
//
//	fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
//	fastcgi_param  SERVER_SOFTWARE    nginx/$nginx_version;
//
//	fastcgi_param  REMOTE_ADDR        $remote_addr;
//	fastcgi_param  REMOTE_PORT        $remote_port;
//	fastcgi_param  REMOTE_USER        $remote_user;
//	fastcgi_param  SERVER_ADDR        $server_addr;
//	fastcgi_param  SERVER_PORT        $server_port;
//	fastcgi_param  SERVER_NAME        $server_name;

	return env;
}

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

void ProcessorImplCgiBinPhp::sendResponse(int clientFd, const std::string &response)
{
	send(clientFd, response.c_str(), response.size(), 0);
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
	std::string basePath = "/tmp";

	return basePath + uri;
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
