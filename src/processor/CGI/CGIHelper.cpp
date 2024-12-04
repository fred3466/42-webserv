#include <ctime>
#include "CGIHelper.h"
#include "../../HttpServer.h"

bool CGIHelper::bChildProcessTimedOut = false;
Harl CGIHelper::harl;
std::string CGIHelper::content = "";
ProcessorAndLocationToProcessor *CGIHelper::nextProcessorAndLocationToProcessor = NULL;
Response *CGIHelper::incompleteResponse = NULL;
Request *CGIHelper::incompleteResponseCorrespondingRequest = NULL;
char *CGIHelper::pendingReqBodyContent = NULL;
int CGIHelper::pendingReqBodyLen = 0;

CGIHelper::CGIHelper() {
	signal(SIGUSR1, _onTimedOutCgiRequest_SIGNAL_HANDLER);
}

CGIHelper::~CGIHelper() {
}

void CGIHelper::feedEnv(char **envp, std::map<std::string, std::string> envMap) {
	int i = 0;

	for (std::map<std::string, std::string>::iterator ite = envMap.begin(); ite != envMap.end(); ++ite) {
		std::string key = ite->first;
		std::string val = ite->second;
		std::string toChar = key + "=" + val;

		int toCharSz = toChar.length();
		char *keyValChars;
		keyValChars = (char*) toChar.c_str();
		keyValChars[toCharSz] = 0;

		envp[i] = new char[toCharSz + 1]();
		memcpy(envp[i], keyValChars, toCharSz + 1);
		envp[i][toCharSz] = 0;
		i++;
	}
	envp[i] = NULL;

}

void CGIHelper::_childProcess(CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap,
		std::string interpreterPath, std::string &scriptPath, Request *request, char **&envp) {
	const char **argv = cgiHandler->buildCommandLine(request, interpreterPath, scriptPath);

	harl.trace("CGIHelper::_childProcess:XXXXX  %i close fd=%i fd_for_RESPONSE[0]", getpid(), pipes->fd_for_RESPONSE[0]);
	harl.trace("CGIHelper::_childProcess: %i dup2 [write end]  fd=%i fd_for_RESPONSE[1]", getpid(), pipes->fd_for_RESPONSE[1]);
	harl.trace("CGIHelper::_childProcess: %i dup2 [read end]  fd=%i fd_for_REQUEST[0]", getpid(), pipes->fd_for_REQUEST[0]);
	harl.trace("CGIHelper::_childProcess:XXXXX  %i close fd=%i fd_for_REQUEST[1]", getpid(), pipes->fd_for_REQUEST[1]);

	if (dup2(pipes->fd_for_RESPONSE[1], STDOUT_FILENO) == -1 //dup the write end of the pipe to stdout to write the response
	|| close(pipes->fd_for_RESPONSE[0]) == -1 || close(pipes->fd_for_RESPONSE[1]) == -1 || dup2(pipes->fd_for_REQUEST[0], STDIN_FILENO) == -1 //dup the read end of the pipe to stdin to read the request
	|| close(pipes->fd_for_REQUEST[1]) == -1 || close(pipes->fd_for_REQUEST[0]) == -1
			) // close the read and write end of the pipe
		throw std::runtime_error("CGIHelper::_childProcess dup2() of close() failed");

//		Contains the current script's path. This is useful for pages which need to point to themselves.
	std::string scriptPathRoot = FileUtil().realPathFile(cgiHandler->getConfig()->getParamStr("base_path", "base_path_missing"));
	(envMap)["DOCUMENT_ROOT"] = scriptPathRoot;

//	PATH_TRANSLATED :	Maps the script's virtual path to the physical path used to call the script. This is done by taking any PATH_INFO component of the request URI and performing any virtual-to-physical translation appropriate.
	std::string pathInfo = request->getUri().getPathInfo();
	std::string PATH_TRANSLATED = FileUtil().realPathFile(pathInfo);
	(envMap)["PATH_TRANSLATED"] = PATH_TRANSLATED;

//	SCRIPT_NAME :	Returns the part of the URL from the protocol name up to the query string in the first line of the HTTP
//	ET selon PHP :
//		Contains the current script's path. This is useful for pages which need to point to themselves. The __FILE__ constant contains the full path and filename of the current (i.e. included) file.
	std::string SCRIPT_NAME = request->getPath() + "/" + request->getFileName() + request->getFileExtension();
	(envMap)["SCRIPT_NAME"] = SCRIPT_NAME;

//	NON défini dans la RFC
//	SCRIPT_FILENAME : Le chemin absolu vers le fichier contenant le script en cours d'exécution.
	(envMap)["SCRIPT_FILENAME"] = scriptPath;

	std::string reqMethod = request->getMethod();
	(envMap)["REQUEST_METHOD"] = reqMethod;

	envp = new char*[envMap.size() + 1];
	feedEnv(envp, envMap);

	if (chdir(scriptPathRoot.c_str()) == 0) {
		const char *execPath = interpreterPath.c_str();
		execve(execPath, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
	}

	harl.debug("%i	CGIHelper::_childProcess EXIT pour pid=%i", getpid(), getpid());
	exit(EXIT_FAILURE); // execve only returns on error
}

void CGIHelper::timedOutChildrenRun(pid_t mainProcessPid, long timeOutUs, CGIHandler *cgiHandler, fdpipe *pipes, std::map<std::string, std::string> envMap, std::string interpreterPath,
		std::string &scriptPath,
		Request *request, char **&envp) {

	pid_t intermediate_pid = fork();
	if (intermediate_pid == 0) {
		pid_t worker_pid = fork();
		if (worker_pid == 0) {
			_childProcess(cgiHandler, pipes, envMap, interpreterPath, scriptPath, request, envp);
			harl.debug("CGIHelper::timedOutChildrenRun: _childProcess EXIT NORMAL %i", getpid());

			return;
		}

		pid_t timeout_pid = fork();
		if (timeout_pid == 0) {
			usleep(timeOutUs);
			harl.debug("CGIHelper::timedOutChildrenRun: TIMEOUT ATTEINT %i", getpid());
		}

		pid_t exited_pid = waitpid(-1, NULL, 0);
		if (exited_pid == worker_pid) {
			harl.trace("CGIHelper::timedOutChildrenRun: KILL KILL KILL OK timeout_pid=%d", timeout_pid);
			kill(timeout_pid, SIGKILL);

		} else {
			harl.debug("CGIHelper::timedOutChildrenRun: KILL KILL KILL KO worker_pid=%d", worker_pid);
			kill(worker_pid, SIGKILL);
			CGIHelper::bChildProcessTimedOut = true;
			kill(mainProcessPid, SIGUSR1);

		}
		harl.trace("CGIHelper::timedOutChildrenRun: fin EXIT %i", getpid());
		return;
	}
	waitpid(intermediate_pid, 0, 0);
	harl.trace("CGIHelper::timedOutChildrenRun: apres waitpid %i", getpid());
}

void CGIHelper::_onTimedOutCgiRequest_SIGNAL_HANDLER(int signal) {
	if (signal == SIGUSR1) {
		CGIHelper::bChildProcessTimedOut = true;

		Response *response = CGIHelper::incompleteResponse;
		if (response) {
			response->setErrorCodeTmp(500);

			nextProcessorAndLocationToProcessor = NULL;
			response->setBNeedFurtherProcessing(false);

			int fd = response->getFd();
			harl.trace("--- %d ServerRunner::_onDataReceiving: setPollFdWritable pour fd=%i", fd);

			Request *request = CGIHelper::incompleteResponseCorrespondingRequest;

			FdPollfdServerTuple *fdPollfdServerTuple = NULL;
			ServerRunner::sendResponse(fdPollfdServerTuple, request, response);
			ServerRunner::bAlreadyDelResped = true;
		}

		CGIHelper::incompleteResponse = NULL;
		CGIHelper::incompleteResponseCorrespondingRequest = NULL;
		HttpServer::freeProcessorList();
	}
}

void CGIHelper::executeCGIScript(CGIHandler *cgiHandler, std::string interpreterPath, std::string &scriptPath, Request *request, Response *response) {
	bChildProcessTimedOut = false;
	content = "";
	std::map<std::string, std::string> envMap = std::map<std::string, std::string>();
	pid_t mainProcessPid = 0;

	setupEnvironmentVariables(&envMap, request, response);

	fdpipe pipes;
	pipe(pipes.fd_for_REQUEST);
	pipe(pipes.fd_for_RESPONSE);

	pid_t pid = fork();

	if (pid == 0) {
		char **envp = NULL;

		timedOutChildrenRun(mainProcessPid, CGI_EXEC_TIMEOUT, cgiHandler, &pipes, envMap, interpreterPath, scriptPath, request, envp);

		int i = 0;
		while (envp && *envp) {
			delete[] envp[i];
		}
		delete[] envp;

		harl.debug("%i	CGIHelper::executeCGIScript EXIT pour pid=%i", getpid(), getpid());
		exit(0);
	} else {
		// Parent process
		_parentProcess(&pipes, request, response);
	}

}

void CGIHelper::_parentProcess(fdpipe *pipes, Request *request, Response *response) {
	RequestBody *reqBody = request->getBody();
	int reqBodyLen = reqBody->getLen();

//	register for reading, on read event "CGIHelper::_onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple)" will be called
	bool bIsInputType = true;
	harl.trace("CGIHelper::_parentProcess: pipe d'entrée de _parentProcess pipes->fd_for_RESPONSE[0]= %i", pipes->fd_for_RESPONSE[0]);
	FdPollfdServerTuple *fdPollfdServerTuple = PollManager::registerPipe(pipes->fd_for_RESPONSE[0], bIsInputType);
	fdPollfdServerTuple->setResponse(response);
	fdPollfdServerTuple->setRequest(request);

	harl.trace("CGIHelper::_parentProcess:XXXXX  %i close fd=%i fd_for_REQUEST[0]", getpid(), pipes->fd_for_REQUEST[0]);
	close(pipes->fd_for_REQUEST[0]); // close the read end of the pipe
	harl.trace("CGIHelper::_parentProcess:XXXXX  %i close fd=%i fd_for_RESPONSE[1]", getpid(), pipes->fd_for_RESPONSE[1]);
	close(pipes->fd_for_RESPONSE[1]);

	if (reqBodyLen > 0) {
		harl.trace("CGIHelper::_parentProcess: pipe de sortie de _parentProcess pipes->fd_for_REQUEST[1] %i", pipes->fd_for_REQUEST[1]);
		bool bIsInputType = false;
		FdPollfdServerTuple *fdPollfdServerTuple = PollManager::registerPipe(pipes->fd_for_REQUEST[1], bIsInputType);
		fdPollfdServerTuple->setResponse(response);

		char *reqBodyContent = reqBody->getContent();
		pendingReqBodyContent = reqBodyContent;
		pendingReqBodyLen = reqBodyLen;
	}
}

bool CGIHelper::_onWritingEnabled(FdPollfdServerTuple *fdPollfdServerTuple) {
	int fdOut = fdPollfdServerTuple->getFd();
	pollfd *pfd = fdPollfdServerTuple->getPollfd();
	pfd->revents &= ~POLLOUT;

	harl.trace("--- %d CGIHelper::_onWritingEnabled: setPollFdWritable pour fd=%i", fdOut);
	int written = write(fdOut, pendingReqBodyContent, pendingReqBodyLen);
	harl.debug("CGIHelper::_onWritingEnabled: WRITE %i after write : %i bytes on fd=%i fd_for_REQUEST[1]", getpid(), written, fdOut);

	harl.trace("CGIHelper::_onWritingEnabled: %i close fd_for_REQUEST[1] fd=%i", getpid(), fdOut);
	PollManager::closeAndReleasePollFd(pfd);

	bool ret = written == pendingReqBodyLen;
	pendingReqBodyContent = NULL;
	pendingReqBodyLen = 0;

	return ret;
}

bool CGIHelper::_onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple) {
	pollfd *inputPollFd = fdPollfdServerTuple->getPollfd();
	inputPollFd->revents &= ~POLLIN;

	int nbRead = 0;
	int nbReadTot = 0;
	char buffer[64 * 1024];
	do {
		std::memset(buffer, 0, 64 * 1024);
		nbRead = read(inputPollFd->fd, buffer, 64 * 1024);
		nbReadTot += nbRead;

		harl.trace("CGIHelper::_onDataReceiving: %i READ %i (%i)> bytes from fd=%i ", getpid(), nbRead, nbReadTot, inputPollFd->fd);
		if (nbRead <= 0) {
			break;
		}
		content.append(buffer, nbRead);

	} while (nbRead > 0);
//		##################################
	harl.debug("CGIHelper::_onDataReceiving: nbReadTot=%i nbReadTot >= 0 ? =>%i", nbReadTot, nbReadTot >= 0);
	harl.trace("CGIHelper::_onDataReceiving:XXXXX  %i close fd=%i fd_for_RESPONSE[1]", getpid(), inputPollFd->fd);

	Response *response = CGIHelper::incompleteResponse;
	if (CGIHelper::bChildProcessTimedOut) {
		response->setErrorCodeTmp(500);
	}

	ProcessorHelper processorHelper;

	Request *request = CGIHelper::incompleteResponseCorrespondingRequest;
	bool bTransferEncoding = true; //"" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
	processorHelper.applyTransferEncodingOrContentLengthAndFinalize(response, &content, bTransferEncoding);

	harl.trace("CGIHelper::_onDataReceiving: CGI 2° passe : appel de HttpServer::runProcessorChain");

	HttpServer::runProcessorChain(request, response, nextProcessorAndLocationToProcessor);

	nextProcessorAndLocationToProcessor = NULL;
	response->setBNeedFurtherProcessing(false);
	HttpServer::freeProcessorList();

	int fd = response->getFd();
	harl.trace("--- %d CGIHelper::_onDataReceiving: setPollFdWritable pour fd=%i", fd);

	ServerRunner::sendResponse(fdPollfdServerTuple, request, response);

	CGIHelper::incompleteResponse = NULL;
	CGIHelper::incompleteResponseCorrespondingRequest = NULL;
	CGIHelper::bChildProcessTimedOut = false;

	return nbReadTot > 0;

}

void CGIHelper::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request, Response *response) {
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
	int CONTENT_LENGTH = request->getBody()->getLen();
	if (!CONTENT_LENGTH || CONTENT_LENGTH <= 0) {
		(*envMap)["CONTENT_LENGTH"] = "";		// request->getHeaderFieldValue("Content-Length"); //visiblement indispensable ! Sinon, voir "TRANSFERT_ENCODING, mutuellement exclusif
	} else {
		(*envMap)["CONTENT_LENGTH"] = su.strFromInt(CONTENT_LENGTH);// request->getHeaderFieldValue("Content-Length"); //visiblement indispensable ! Sinon, voir "TRANSFERT_ENCODING, mutuellement exclusif
	}

//	CONTENT_TYPE : The MIME type of the body of the request, or null if the type is not known. For HTTP servlets, the value returned is the same as the value of the CGI variable CONTENT_TYPE.
	(*envMap)["CONTENT_TYPE"] = ""; //voir processor common et ailleurs

//	GATEWAY_INTERFACE : What revision of the CGI specification the server is using; e.g. 'CGI/1.1'
	(*envMap)["GATEWAY_INTERFACE"] = "CGI/1.1";

//	PATH_INFO : Optionally contains extra path information from the HTTP request that invoked the script,
//	specifying a path to be interpreted by the CGI script.
//	PATH_INFO identifies the resource or sub-resource to be returned by the CGI script,
//	and it is derived from the portion of the URI path following the script name but preceding any query data.
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
	(*envMap)["DOCUMENT_URI"] = request->getUri().getUri();
//	Set to a non-empty value if the script was queried through the HTTPS protocol.
	(*envMap)["HTTPS"] = "";

//	DEBUT transfert des header de la Request vers le header de la Response, préfixés par "HTTP_", en respect de CGI-BIN
	std::list<std::string> request_headers = request->getHeader()->getFields();
	for (std::list<std::string>::iterator it = request_headers.begin(); it != request_headers.end(); ++it) {
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

std::string CGIHelper::getContent()
{
	return content;
}
