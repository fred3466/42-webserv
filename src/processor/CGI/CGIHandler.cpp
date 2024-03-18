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

std::string CGIHandler::executeCGI(const std::string &scriptPath,
		const std::map<std::string, std::string> &envVariables)
{
	// Set environment variables
	for (std::map<std::string, std::string>::const_iterator it = envVariables.begin(); it != envVariables.end(); ++it)
	{
		setenv(it->first.c_str(), it->second.c_str(), 1);
	}

	// Execute CGI script and capture the output
	std::string command = "exec " + scriptPath;
	FILE *pipe = popen(command.c_str(), "r");
	if (!pipe)
		return "ERROR";

	char buffer[128];
	std::string result = "";
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

	int status = pclose(pipe);
	if (WIFEXITED(status))
	{
		int exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0)
		{
			// Handle non-zero exit status as an error
			std::ostringstream oss;
			oss << exitStatus;
			logError("CGI script exited with error status: " + oss.str());
			return "ERROR";
		}
	}
	else
	{
		// Handle cases where the script didn't exit normally
		logError("CGI script did not exit normally.");
		return "ERROR";
	}

	// Reset environment variables
	for (std::map<std::string, std::string>::const_iterator it = envVariables.begin(); it != envVariables.end(); ++it)
	{
		unsetenv(it->first.c_str());
	}

	// Parse and store the CGI output
	parseOutput(result);

	return responseBody;
}

void CGIHandler::parseOutput(const std::string &output)
{
	std::istringstream stream(output);
	std::string line;
	bool headerSection = true; // Start by parsing headers

	responseHeaders.clear(); // Clear previous headers if any
	responseBody.clear();    // Clear previous body if any

	while (std::getline(stream, line))
	{
		// Check and remove carriage return at the end if present
		if (!line.empty() && line[line.length() - 1] == '\r')
		{
			line.erase(line.length() - 1);
		}

		if (headerSection)
		{
			// Check for the empty line indicating end of headers
			if (line.empty())
			{
				headerSection = false; // Headers end, body starts
				continue;
			}

			// Parse header line
			std::size_t pos = line.find(":");
			if (pos != std::string::npos)
			{
				std::string headerName = line.substr(0, pos);
				std::string headerValue = line.substr(pos + 2); // Skip ": "
				responseHeaders[headerName] = headerValue;
			}
		}
		else
		{
			// The first line of the body will be an empty line, skip it
			if (line.empty())
			{
				continue;
			}
			// The rest of the stream is the body
			responseBody.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
			break; // Exit after body is read
		}
	}
}

void CGIHandler::logError(const std::string &message)
{
	std::ofstream logFile("error_log.txt", std::ios::app); // Append to the log file
	if (logFile)
	{
		logFile << message << std::endl;
	}
}

void CGIHandler::logSuccess(const std::string &message)
{
	std::ofstream logFile("access_log.txt", std::ios::app); // Append to the log file
	if (logFile)
	{
		logFile << message << std::endl;
	}
}

void CGIHandler::setupEnvironmentVariables(std::map<std::string, std::string> *envMap, Request *request,
		Response *response)
{
//	Some server supplied environment variables are not defined in the current » CGI/1.1 specification. Only the following variables are defined there: AUTH_TYPE, CONTENT_LENGTH, CONTENT_TYPE, GATEWAY_INTERFACE, PATH_INFO, PATH_TRANSLATED, QUERY_STRING, REMOTE_ADDR, REMOTE_HOST, REMOTE_IDENT, REMOTE_USER, REQUEST_METHOD, SCRIPT_NAME, SERVER_NAME, SERVER_PORT, SERVER_PROTOCOL, and SERVER_SOFTWARE. Everything else should be treated as 'vendor extensions'.

	// Environment variables
	(*envMap)["REQUEST_METHOD"] = request->getMethod();
	(*envMap)["QUERY_STRING"] = request->getQueryString();
	(*envMap)["CONTENT_TYPE"] = request->getHeaderFieldValue("Content-Type");
//	TODO urgent !
	(*envMap)["CONTENT_LENGTH"] = request->getHeaderFieldValue("Content-Length");
	(*envMap)["GATEWAY_INTERFACE"] = "CGI/1.1";
	(*envMap)["SERVER_PROTOCOL"] = "HTTP/1.1";
//	env["SCRIPT_FILENAME"] = "";
//	env["SCRIPT_NAME"] = "";
	(*envMap)["REDIRECT_STATUS"] = "200";

	(*envMap)["REQUEST_URI"] = request->getUri();
	(*envMap)["DOCUMENT_URI"] = request->getUri(); //TODO a voir
	(*envMap)["DOCUMENT_ROOT"] = request->getUri(); //TODO a voir
	(*envMap)["SERVER_PROTOCOL"] = "http://"; //TODO a voir
	(*envMap)["REQUEST_SCHEME"] = "http"; //TODO a voir
	(*envMap)["HTTPS"] = ""; //TODO a voir
	(*envMap)["SERVER_SOFTWARE"] = "webserv/Anastasia Jean-Baptiste Frédéric";
	(*envMap)["REMOTE_ADDR"] = ""; //TODO a voir
	(*envMap)["REMOTE_PORT"] = ""; //TODO a voir
	(*envMap)["REMOTE_USER"] = ""; //TODO a voir
	(*envMap)["SERVER_ADDR"] = ""; //TODO a voir
	(*envMap)["SERVER_PORT"] = ""; //TODO a voir
	(*envMap)["SERVER_NAME"] = "mon serveur web"; //TODO a voir
	(*envMap)["SERVER_PORT"] = ""; //TODO a voir

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
//		(envMap)["SCRIPT_FILENAME"] = scriptPath;
//		TODO SCRIPT_NAME
//		(envMap)["SCRIPT_NAME"] = scriptPath;

		char *envp[envMap.size()];
		int i = 0;
		for (std::map<std::string, std::string>::iterator ite = envMap.begin(); ite != envMap.end(); ++ite)
		{
			std::string key = ite->first;
			std::string val = ite->second;
			std::string toChar = key + "=" + val;
//			TODO leak
			envp[i] = new char[toChar.length()]();
			memcpy((char*) envp[i], toChar.c_str(), toChar.length() + 1);
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

std::string CGIHandler::captureScriptOutput(int fileDescriptor)
{
	std::stringstream output;
	char buffer[1024];
	ssize_t bytesRead;

	while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytesRead] = '\0';
		output << buffer;
	}

	close(fileDescriptor);
	return output.str();
}
