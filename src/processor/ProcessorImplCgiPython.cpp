#include "ProcessorImplCgiPython.h"

ProcessorImplCgiPython::ProcessorImplCgiPython(ProcessorTypeEnum type) : harl(), stringUtil(), config()
{
	this->type = type;
}

ProcessorImplCgiPython::~ProcessorImplCgiPython()
{
}

void ProcessorImplCgiPython::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiPython::process(Request *request)
{
	FileUtil *fu = FileUtilFactory().build();

	std::string basePath = config->getParamStr("base_path", "base_path");
	std::string path = basePath + request->getUri();
	harl.info(request->getUri() + " -> " + path);
	char *body;

	if (isCGIRequest(request->getUri()))
	{
		CGIHandler cgiHandler;

		std::map<std::string, std::string> envVars = prepareCGIEnvironment(request);

		std::string scriptPath = getScriptPath(request->getUri());

		std::string cgiOutput = cgiHandler.executeCGIScript(scriptPath, envVars, request->getMethod(),
				request->getQueryString());

		std::string httpResponse = generateHttpResponse(cgiOutput);
	}

	return response;
}

bool ProcessorImplCgiPython::isCGIRequest(const std::string &uri)
{
	return uri.find("/cgi-bin/") == 0;
}

std::map<std::string, std::string> ProcessorImplCgiPython::prepareCGIEnvironment(Request *request)
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

ProcessorTypeEnum ProcessorImplCgiPython::getType()
{
	return type;
}
