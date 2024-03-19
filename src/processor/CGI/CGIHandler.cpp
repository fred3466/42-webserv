#include "CGIHandler.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>
#include <cstring>

CGIHandler::CGIHandler() : harl()
{
}

CGIHandler::~CGIHandler()
{
}

void CGIHandler::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request,
		Response *response)
{
//	Some server supplied environment variables are not defined in the current » CGI/1.1 specification. Only the following variables are defined there: AUTH_TYPE, CONTENT_LENGTH, CONTENT_TYPE, GATEWAY_INTERFACE, PATH_INFO, PATH_TRANSLATED, QUERY_STRING, REMOTE_ADDR, REMOTE_HOST, REMOTE_IDENT, REMOTE_USER, REQUEST_METHOD, SCRIPT_NAME, SERVER_NAME, SERVER_PORT, SERVER_PROTOCOL, and SERVER_SOFTWARE. Everything else should be treated as 'vendor extensions'.

// Environment variables
//	Which request method was used to access the page; e.g. 'GET', 'HEAD', 'POST', 'PUT'.
	(*envMap)["REQUEST_METHOD"] = request->getMethod();
//	The query string, if any, via which the page was accessed.
	(*envMap)["QUERY_STRING"] = request->getQueryString();
	(*envMap)["CONTENT_TYPE"] = request->getHeaderFieldValue("Content-Type");

//   The Content-Length entity-header field indicates the size of the
//   entity-body, in decimal number of OCTETs, sent to the recipient or,
//   in the case of the HEAD method, the size of the entity-body that
//   would have been sent had the request been a GET.
//
//       Content-Length    = "Content-Length" ":" 1*DIGIT
//
//   An example is
//
//       Content-Length: 3495
//
//   Applications SHOULD use this field to indicate the transfer-length of
//   the message-body, unless this is prohibited by the rules in section
//   4.4.
//	(*envMap)["CONTENT_LENGTH"] = request->getHeaderFieldValue("Content-Length");
//	What revision of the CGI specification the server is using; e.g. 'CGI/1.1'
	(*envMap)["GATEWAY_INTERFACE"] = "CGI/1.1";
	(*envMap)["SERVER_PROTOCOL"] = "HTTP/1.1";
//	env["SCRIPT_FILENAME"] = "";
//	env["SCRIPT_NAME"] = "";
	(*envMap)["REDIRECT_STATUS"] = "200";

//	The URI which was given in order to access this page; for instance, '/index.html'.
	(*envMap)["REQUEST_URI"] = request->getUri();
	(*envMap)["DOCUMENT_URI"] = request->getUri(); //TODO a voir
//	The document root directory under which the current script is executing, as defined in the server's configuration file.
	(*envMap)["DOCUMENT_ROOT"] = request->getUri(); //TODO a voir
//	Name and revision of the information protocol via which the page was requested; e.g. 'HTTP/1.0';
	(*envMap)["REQUEST_SCHEME"] = "http"; //TODO a voir
//	Set to a non-empty value if the script was queried through the HTTPS protocol.
	(*envMap)["HTTPS"] = ""; //TODO a voir
//	Server identification string, given in the headers when responding to requests.
	(*envMap)["SERVER_SOFTWARE"] = "webserv/Anastasia Jean-Baptiste Frédéric";
//	The IP address from which the user is viewing the current page.
	(*envMap)["REMOTE_ADDR"] = ""; //TODO a voir
//	The port being used on the user's machine to communicate with the web server.
	(*envMap)["REMOTE_PORT"] = ""; //TODO a voir
//	The authenticated user.
	(*envMap)["REMOTE_USER"] = ""; //TODO a voir
//	The IP address of the server under which the current script is executing.
	(*envMap)["SERVER_ADDR"] = ""; //TODO a voir
//	The port on the server machine being used by the web server for communication. For default setups, this will be '80'; using SSL, for instance, will change this to whatever your defined secure HTTP port is.
	(*envMap)["SERVER_PORT"] = ""; //TODO a voir
//	The name of the server host under which the current script is executing. If the script is running on a virtual host, this will be the value defined for that virtual host.
	(*envMap)["SERVER_NAME"] = "mon serveur web"; //TODO a voir
	(*envMap)["SERVER_PORT"] = ""; //TODO a voir

//	'REQUEST_TIME'
//	    The timestamp of the start of the request.
//	'REQUEST_TIME_FLOAT'
//	    The timestamp of the start of the request, with microsecond precision.
//	'REMOTE_HOST'
//	    The Host name from which the user is viewing the current page. The reverse dns lookup is based on the REMOTE_ADDR of the user.
//	REDIRECT_REMOTE_USER
//	The authenticated user if the request is internally redirected.
//	SERVER_ADMIN'
//	    The value given to the SERVER_ADMIN (for Apache) directive in the web server configuration file. If the script is running on a virtual host, this will be the value defined for that virtual host.
//	SERVER_SIGNATURE'
//	    String containing the server version and virtual host name which are added to server-generated pages, if enabled.
//	PATH_TRANSLATED'
//	    Filesystem- (not document root-) based path to the current script, after the server has done any virtual-to-real mapping.
//	PATH_INFO'
//	    Contains any client-provided pathname information trailing the actual script filename but preceding the query string, if available. For instance, if the current script was accessed via the URI http://www.example.com/php/path_info.php/some/stuff?foo=bar, then $_SERVER['PATH_INFO'] would contain /some/stuff.
//	ORIG_PATH_INFO'
//	    Original version of 'PATH_INFO' before processed by PHP.

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

}

std::string CGIHandler::executeCGIScript(std::string interpreterPath, std::string &scriptPath, Request *request,
		Response *response)
{
// Setup environment variables
	std::map<std::string, std::string> envMap = std::map<std::string, std::string>();
	setupEnvironmentVariables(&envMap, request, response);

	int pipefd[2];
	pipe(pipefd); // Create a pipe

	pid_t pid = fork();
	if (pid == 0)
	{
		// Child process
		std::string cmdLine = "-f " + scriptPath;
//		TODO SCRIPT_NAME et SCRIPT_FILENAME à verifier
//		(envMap)["SCRIPT_FILENAME_XXXXXXXXXXXXXX"] = request->getFileName();
//		The absolute pathname of the currently executing script.
		(envMap)["SCRIPT_FILENAME"] = request->getFileName();

//		Contains the current script's path. This is useful for pages which need to point to themselves. The __FILE__ constant contains the full path and filename of the current (i.e. included) file.
		(envMap)["SCRIPT_NAME"] = scriptPath;

		char **envp = new char*[envMap.size()]; //need to be delete() after it is used, or else it will cause memory leak
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
			harl.debug("CGIHandler::executeCGIScript env script\n[%s=%s] => [%s]", key.c_str(), val.c_str(), envp[i]);
			i++;
		}
//		std::string toChar = "toto=titi";
//		//			TODO leak
//		envp[i] = new char[toChar.length()];
//		memcpy((char*) envp[i], toChar.c_str(), toChar.length());
//		i++;
		envp[i] = NULL;

		harl.debug("CGIHandler::executeCGIScript [%s %s]", interpreterPath.c_str(), cmdLine.c_str());
		close(pipefd[0]);               // Close unused read end
		dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
//	execl(interpreterPath.c_str(), cmdLine.c_str(), (char*) NULL);

		const char *argv[3];
		i = 0;
		argv[i++] = interpreterPath.c_str();
		argv[i++] = "-f";
		std::string stp = scriptPath;
//		std::string stp = "\"" + scriptPath + "\"";
		argv[i++] = stp.c_str();
		argv[i++] = NULL;

		const char *execPath = interpreterPath.c_str();

		execve(execPath, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
//		execv(execPath, const_cast<char* const*>(argv));
		exit(EXIT_FAILURE); // execl only returns on error
	}
	else
	{
		// Parent process
		close(pipefd[1]); // Close unused write end

		char buffer[1024] =
				{ };
		std::string output;
		ssize_t count;
		while ((count = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[count] = '\0';
			output += buffer;
		}
		close(pipefd[0]); // Close read end

		// Wait for child process to terminate
		int status;
		waitpid(pid, &status, 0);

		// Reset environment variables if needed here
		harl.debug("CGIHandler::executeCGIScript: output :\n%s", output.c_str());
		return output;
	}
}

