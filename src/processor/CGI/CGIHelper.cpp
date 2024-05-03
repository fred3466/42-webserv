#include "CGIHelper.h"

CGIHelper::CGIHelper() : harl()
{
}

CGIHelper::~CGIHelper()
{
}

void CGIHelper::_childProcess(CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath, std::string &scriptPath, Request *request, char **envp)
{
	const char **argv = cgiHandler->buildCommandLine(interpreterPath, scriptPath);

	if (dup2(pipes->fd_for_RESPONSE[1], STDOUT_FILENO) == -1 //dup the write end of the pipe to stdout to write the response
	|| close(pipes->fd_for_RESPONSE[0]) == -1 || close(pipes->fd_for_RESPONSE[1]) == -1
			|| dup2(pipes->fd_for_REQUEST[0], STDIN_FILENO) == -1 //dup the read end of the pipe to stdin to read the request
			|| close(pipes->fd_for_REQUEST[1]) == -1 || close(pipes->fd_for_REQUEST[0]) == -1) // close the read and write end of the pipe
		throw std::runtime_error("CGIHelper::_childProcess dup2() of close() failed");

//		Contains the current script's path. This is useful for pages which need to point to themselves. The __FILE__ constant contains the full path and filename of the current (i.e. included) file.
	std::string scriptPathRoot = FileUtil().realPathFile(cgiHandler->getConfig()->getParamStr("base_path", "base_path_missing"));
	(envMap)["DOCUMENT_ROOT"] = scriptPathRoot;

//	TODO httperror si pathInfo n'est pas un chemin valide
//	PATH_TRANSLATED :	Maps the script's virtual path to the physical path used to call the script. This is done by taking any PATH_INFO component of the request URI and performing any virtual-to-physical translation appropriate.
	std::string pathInfo = request->getUri().getPathInfo();
	std::string PATH_TRANSLATED = FileUtil().realPathFile(pathInfo);
	(envMap)["PATH_TRANSLATED"] = PATH_TRANSLATED;

//	SCRIPT_NAME :	Returns the part of the URL from the protocol name up to the query string in the first line of the HTTP
//	ET selon PHP :
//		Contains the current script's path. This is useful for pages which need to point to themselves. The __FILE__ constant contains the full path and filename of the current (i.e. included) file.
	std::string SCRIPT_NAME = request->getPath() + request->getFileName() + request->getFileExtension();
	(envMap)["SCRIPT_NAME"] = SCRIPT_NAME;

//	NON défini dans la RFC
//	SCRIPT_FILENAME : Le chemin absolu vers le fichier contenant le script en cours d'exécution.
	(envMap)["SCRIPT_FILENAME"] = scriptPath;

	feedEnv(envp, envMap);

	const char *execPath = interpreterPath.c_str();
	execve(execPath, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
	exit(EXIT_FAILURE); // execve only returns on error
}

std::string
CGIHelper::executeCGIScript(CGIHandler *cgiHandler, std::string interpreterPath, std::string &scriptPath, Request *request,
		Response *response)
{
	std::string output;
	std::map<std::string, std::string> envMap = std::map<std::string, std::string>();
	setupEnvironmentVariables(&envMap, request, response);

	fdpipe pipes;
	pipe(pipes.fd_for_REQUEST);
	pipe(pipes.fd_for_RESPONSE);

	pid_t pid = fork();

	if (pid == 0)
	{
		char **envp = new char*[envMap.size() + 1]; //need to be delete() after it is used, or else it will cause memory leak
		_childProcess(cgiHandler, &pipes, envMap, interpreterPath, scriptPath, request, envp);
//TODO truc à faire ici ?
//		delete envp[envMap.size()];
	}
	else
	{
		// Parent process
		_parentProcess(&output, &pipes, request, pid);
//		harl.debug("CGIHelper::executeCGIScript: output :\n%s", output.c_str());
		return output;
	}
//	int status;
//	waitpid(pid, &status, 0);

//	int status;
//	wait(&status);

	return output;
}

void CGIHelper::_parentProcess(std::string *output, fdpipe *pipes, Request *request, int pid)
{
	RequestBody *reqBody = request->getBody();
	std::string reqBodyContent = std::string(reqBody->getContent());
//	std::string *reqBodyContentPtr = &reqBodyContent;
//	std::string reqBodyContent = *reqBodyContentStr;

//	close(pipes->fd_for_REQUEST[1]); // !!!!
//	close(pipes->fd_for_RESPONSE[0]);

	close(pipes->fd_for_REQUEST[0]); // close the read end of the pipe
	if (reqBodyContent != "")
	{
		char *cstr = (char*) (reqBodyContent.c_str());
		int length = (int) reqBodyContent.size();

		int written = write(pipes->fd_for_REQUEST[1], cstr, length);
//		int written = write(pipes->fd_for_RESPONSE[1], cstr, length);
		(void) written;
//			harl.debug("CGIHelper::executeCGIScript: %d sent %d bytes through the wire", pipefd_pere_fils[1], written);
	}
	close(pipes->fd_for_REQUEST[1]);
	close(pipes->fd_for_RESPONSE[1]);
//	sleep(1);

	int status; // !!!!
	waitpid(pid, &status, 0); // !!!!
//	(void) pid;

	//	int bufferReadSize = 1024;
//	char *buffer = new char[bufferReadSize]();
//	std::memset(buffer, 0, bufferReadSize);

	int ret = 0;
	char buffer[1024];
	do
	{
		std::memset(buffer, 0, 1024);
		ret = read(pipes->fd_for_RESPONSE[0], buffer, 1024);
		(*output).append(buffer, ret);

	} while (ret > 0);

//	ssize_t count;
//	while ((count = read(pipes->fd_for_RESPONSE[0], buffer, bufferReadSize)) > 0)
//	{
//		*output += buffer;
//		std::memset(buffer, 0, bufferReadSize);
//	}
	close(pipes->fd_for_RESPONSE[0]); // !!!!

//	sleep(1);
}

void CGIHelper::feedEnv(char **envp, std::map<std::string, std::string> envMap)
{
	int i = 0;
	for (std::map<std::string, std::string>::iterator ite = envMap.begin(); ite != envMap.end(); ++ite)
	{
		std::string key = ite->first;
		std::string val = ite->second;
		std::string toChar = key + "=" + val;
//			TODO leak
//			TODO BUG!!!!
		int toCharSz = toChar.length();
		envp[i] = new char[toCharSz + 1]();
		memcpy((char*) envp[i], toChar.c_str(), toCharSz + 1);
		envp[i][toCharSz] = 0;
		harl.trace("CGIHelper::feedEnv env script\n[%s=%s] => [%s]", key.c_str(), val.c_str(), envp[i]);
		i++;
	}
	envp[i] = NULL;

}
void CGIHelper::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response)
{
	(void) response;
	/*
	 Some server supplied environment variables are not defined in the current » CGI/1.1 specification. Only the following variables are defined there:
	 AUTH_TYPE, CONTENT_LENGTH, CONTENT_TYPE, GATEWAY_INTERFACE, PATH_INFO, PATH_TRANSLATED, QUERY_STRING,
	 REMOTE_ADDR, REMOTE_HOST, REMOTE_IDENT, REMOTE_USER, REQUEST_METHOD, SCRIPT_NAME, SERVER_NAME,
	 SERVER_PORT, SERVER_PROTOCOL, and SERVER_SOFTWARE.
	 Everything else should be treated as 'vendor extensions'.

	 Mis dans _childProcess : DOCUMENT_ROOT, PATH_TRANSLATED, SCRIPT_NAME, SCRIPT_FILENAME
	 */
	StringUtil su = StringUtil();
	/*
	 ------------------------- DEBUT STANDARD -------------------------
	 d'après:
	 https://www.ibm.com/docs/en/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script
	 et
	 https://www.php.net/manual/fr/reserved.variables.server.php
	 */

//	AUTH_TYPE : The name of the authentication scheme used to protect the servlet. For example, BASIC, SSL, or null if the servlet was not protected.
	(*envMap)["AUTH_TYPE"] = ""; //hors sujet pour webserv

	//	CONTENT_LENGTH : The length of the request body in bytes made available by the input stream or -1 if the length is not known. For HTTP servlets, the value returned is the same as the value of the CGI variable CONTENT_LENGTH.
	(*envMap)["CONTENT_LENGTH"] = request->getHeaderFieldValue("Content-Length"); //visiblement indispensable ! Sinon, voir "TRANSFERT_ENCODING, mutuellement exclusif

//	CONTENT_TYPE : The MIME type of the body of the request, or null if the type is not known. For HTTP servlets, the value returned is the same as the value of the CGI variable CONTENT_TYPE.
	(*envMap)["CONTENT_TYPE"] = ""; //voir processor common et ailleurs

//	GATEWAY_INTERFACE : What revision of the CGI specification the server is using; e.g. 'CGI/1.1'
	(*envMap)["GATEWAY_INTERFACE"] = "CGI/1.1";

//	PATH_INFO : Optionally contains extra path information from the HTTP request that invoked the script, specifying a path to be interpreted by the CGI script. PATH_INFO identifies the resource or sub-resource to be returned by the CGI script, and it is derived from the portion of the URI path following the script name but preceding any query data.
	std::string pathInfo = request->getUri().getPathInfo();
	(*envMap)["PATH_INFO"] = pathInfo;

//	QUERY_STRING : The query string, if any, via which the page was accessed.
	(*envMap)["QUERY_STRING"] = request->getQueryString();

//	REMOTE_ADDR : The IP address from which the user is viewing the current page.
	(*envMap)["REMOTE_ADDR"] = request->getRemoteIpPort();

//	REMOTE_HOST : The fully-qualified name of the client that sent the request, or the IP address of the client if the name cannot be determined. For HTTP servlets, the value returned is the same as the value of the CGI variable REMOTE_HOST.
//	Pour webserv, on renvoit toujours l'IP, pas de query DNS
	(*envMap)["REMOTE_HOST"] = request->getRemoteIpPort();

//	REMOTE_IDENT : inconnu au bataillon ?
	(*envMap)["REMOTE_IDENT"] = "";

//	REMOTE_USER : Returns the login of the user making this request if the user has been authenticated, or null if the user has not been authenticated.
	(*envMap)["REMOTE_USER"] = "";

//	REQUEST_METHOD : Returns the name of the HTTP method with which this request was made. For example, GET, POST, or PUT.
	std::string reqMethod = request->getMethod();
	(*envMap)["REQUEST_METHOD"] = reqMethod;

//	SERVER_NAME : Returns the host name of the server that received the request. For HTTP servlets, it is the same as the value of the CGI variable SERVER_NAME.
	(*envMap)["SERVER_NAME"] = request->getHost();

//	SERVER_PORT : Returns the port number on which this request was received. For HTTP servlets, the value returned is the same as the value of the CGI variable SERVER_PORT
	std::string serverPort = su.strFromInt(request->getPort());
	(*envMap)["SERVER_PORT"] = serverPort;

//	SERVER_PROTOCOL : Returns the name and version of the protocol the request uses in the following form: protocol/majorVersion.minorVersion. For example, HTTP/1.1. For HTTP servlets, the value returned is the same as the value of the CGI variable SERVER_PROTOCOL.
	(*envMap)["SERVER_PROTOCOL"] = "HTTP/1.1";

//	SERVER_SOFTWARE : Server identification string, given in the headers when responding to requests.
	(*envMap)["SERVER_SOFTWARE"] = request->getHeaderFieldValue("Server"); //"webserv/Anastasia Jean-Baptiste Frédéric";

//	PATH_TRANSLATED : Maps the script's virtual path to the physical path used to call the script. This is done by taking any PATH_INFO component of the request URI and performing any virtual-to-physical translation appropriate.
	(*envMap)["PATH_TRANSLATED"] = ""; // voir _childProcess

	/*
	 ------------------------- FIN STANDARD -------------------------
	 */
	(*envMap)["REDIRECT_STATUS"] = "200";

	//	The URI which was given in order to access this page; for instance, '/index.html'.
	(*envMap)["REQUEST_URI"] = request->getUri().getUri();
	(*envMap)["DOCUMENT_URI"] = request->getUri().getUri(); // TODO a voir
	//	Set to a non-empty value if the script was queried through the HTTPS protocol.
	(*envMap)["HTTPS"] = "";

//	DEBUT transfert des header de la Request vers le header de la Response, préfixés par "HTTP_", en respect de CGI-BIN
	std::list<std::string> request_headers = request->getHeader()->getFields();
	for (std::list<std::string>::iterator it = request_headers.begin();
			it != request_headers.end(); ++it)
	{
		std::string rawField = *it;
		int nbSeparatorsToProcess = 1;
		std::vector<std::string> toks = su.tokenize(rawField, ':', nbSeparatorsToProcess);
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		name = su.strUpperCase(std::string("HTTP_") + su.trim(name));
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		val = su.trim(val);
		val = su.replace_all(val, ".", "_");
		val = su.replace_all(val, "-", "_");

		(*envMap)[name] = val;
	}
// FIN	transfert des header de la Request vers le header de la Response, préfixés par "HTTP_", en respect de CGI-BIN

}
