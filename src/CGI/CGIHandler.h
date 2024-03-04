#pragma once

#include <string>
#include <map>

class CGIHandler
{
private:
    // Parses the CGI script output
    void parseOutput(const std::string &output);

    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;
    void setupEnvironmentVariables(const std::map<std::string,
                                                  std::string> &requestHeaders,
                                   const std::string &requestMethod,
                                   const std::string &queryString);

    std::string captureScriptOutput(int fileDescriptor);

public:
    CGIHandler();
    virtual ~CGIHandler();

    // Executes the CGI script and returns the result
    std::string executeCGI(const std::string &scriptPath, const std::map<std::string, std::string> &envVariables);
    void logError(const std::string &message);
    void logSuccess(const std::string &message);
    std::string executeCGIScript(const std::string &scriptPath);
};
