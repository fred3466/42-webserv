#include "../request/HttpRequest.h"
#include <istream>
#include <iterator>
#include "Request.h"

/*
constructor takes a argument std::string  requestText,
which is expected to contain the raw text of an HTTP request.
Constructor performs the operations to parse raw request into parts:
Creates an std::istringstream parse the string line by line or part by part
Parses the Request Line: extract the HTTP method , the request URL,
and the HTTP version from the first line of the request.
parseHeaders(requestStream)  read headers from the subsequent lines of the input stream.
Headers in the format Header-Name: Header-Value, with each header separated by a newline.
Function continues reading headers until it encounters a blank line,
which signifies the end of the headers section according to the HTTP protocol.
parseBody function reads the remainder of the requestStream as the body of the request
*/

HttpRequest::HttpRequest(const std::string &requestText)
{
    std::istringstream requestStream(requestText);
    parseRequestLine(requestStream);
    parseHeaders(requestStream);
    if (method == "POST")
    {
        parseBody(requestStream);
    }
}

/*
The method, URL, and HTTP version are separated by single spaces and that the line
ends with a \r and \n. By dividing the request line into these three components,
the function prepares the HttpRequest object with the essential information it needs
to handle the request.
*/
void HttpRequest::parseRequestLine(std::istringstream &stream)
{
    std::getline(stream, method, ' ');
    std::getline(stream, url, ' ');
    std::getline(stream, httpVersion);
    httpVersion = httpVersion.substr(0, httpVersion.size() - 1); // Remove \r
}

void HttpRequest::parseHeaders(std::istringstream &stream)
{
    std::string headerLine;
    while (std::getline(stream, headerLine) && headerLine != "\r")
    {
        std::string::size_type colonPos = headerLine.find(":");
        if (colonPos != std::string::npos)
        {
            std::string headerName = headerLine.substr(0, colonPos);
            std::string headerValue = headerLine.substr(colonPos + 2,
                                                        headerLine.size() - colonPos - 3); // Remove \r
            headers[headerName] = headerValue;
        }
    }
}

/*
extract and store the body of an HTTP request from a given input stream
Stream to String Conversion: creates an iterator that reads characters from the stream.
When used without a specific end position, it reads until the end of the stream.
Assigning to Body: The assign function of the std::string class replaces the current content
of body with the new content defined by the range of iterators provided
*/
void HttpRequest::parseBody(std::istringstream &stream)
{
    body.assign(std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>());
}

// Gets the value of a specific parameter by name
std::string HttpRequest::getValue(std::string paramName) const
{
    std::map<std::string, std::string>::const_iterator it = headers.find(paramName);
    if (it != headers.end())
    {
        return it->second;
    }
    return ""; // Parameter not found
}

// Adds a parameter to the request.
void HttpRequest::addParam(std::string paramName, std::string paramValue)
{
    // directly adds to headers
    headers[paramName] = paramValue;
}

// Returns the request URI
std::string HttpRequest::getUri() const
{
    return url;
}

// Returns the HTTP method
std::string HttpRequest::getMethod() const
{
    return method;
}

// The request contents for debugging
void HttpRequest::dump() const
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void HttpRequest::setFdClient(int fd)
{
    fdClient = fd;
}

int HttpRequest::getFdClient() const
{
    return fdClient;
}