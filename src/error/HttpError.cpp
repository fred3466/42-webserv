#include "HttpError.h"

HttpError::HttpError(int code, const std::string &description)
    : code(code), description(description) {}

HttpError::~HttpError() {}

void HttpError::setCode(int newCode)
{
    code = newCode;
}

void HttpError::setDescription(const std::string &newDescription)
{
    description = newDescription;
}

int HttpError::getCode() const
{
    return code;
}

std::string HttpError::getDescription() const
{
    return description;
}

std::string HttpError::getStatusLine() const
{
    std::ostringstream ssHttpReturnCode;
    ssHttpReturnCode << code;
    std::string statusLine = "HTTP/1.1 " + ssHttpReturnCode.str() + " " + description + "\r\n";
    return statusLine;
}
