#include "CGIHandler.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>

CGIHandler::CGIHandler()
{
}

CGIHandler::~CGIHandler()
{
}

std::string CGIHandler::executeCGI(const std::string &scriptPath, const std::map<std::string, std::string> &envVariables)
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

void CGIHandler::setupEnvironmentVariables(const std::map<std::string, std::string> &requestHeaders,
                                           const std::string &requestMethod,
                                           const std::string &queryString)
{
    // Environment variables
    setenv("REQUEST_METHOD", requestMethod.c_str(), 1);
    setenv("QUERY_STRING", queryString.c_str(), 1);
    setenv("REDIRECT_STATUS", "200", 1);
}

std::string CGIHandler::executeCGIScript(const std::string &scriptPath, const std::map<std::string, std::string> &requestHeaders, const std::string &requestMethod, const std::string &queryString)
{
    // Setup environment variables
    setupEnvironmentVariables(requestHeaders, requestMethod, queryString);

    int pipefd[2];
    pipe(pipefd); // Create a pipe

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        close(pipefd[0]);               // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        execl(scriptPath.c_str(), scriptPath.c_str(), (char *)NULL);
        exit(EXIT_FAILURE); // execl only returns on error
    }
    else
    {
        // Parent process
        close(pipefd[1]); // Close unused write end

        char buffer[1024];
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
