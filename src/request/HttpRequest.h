#pragma once
#include "Request.h"
#include <istream>
#include <iterator>
#include <iostream>

class HttpRequest : public Request
{
private:
    std::string method;
    std::string url;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string body;
    int fdClient;

    void parseRequestLine(std::istringstream &stream);
    void parseHeaders(std::istringstream &stream);
    void parseBody(std::istringstream &stream);

public:
    HttpRequest(const std::string &requestText);
    virtual ~HttpRequest(){};

    virtual std::string getValue(std::string paramName) const;
    virtual void addParam(std::string paramName, std::string paramValue);
    virtual std::string getUri() const;
    virtual std::string getMethod() const;
    virtual void dump() const;
    virtual void setFdClient(int fd);
    virtual int getFdClient() const;
};
