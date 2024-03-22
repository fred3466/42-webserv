#include "FiltreError.h"

// FiltreError::FiltreError(/* args */)
// {
// }

FiltreError::~FiltreError()
{
}

FiltreError::FiltreError(ProcessorTypeEnum type)
{
    this->type = type;
}

Response *FiltreError::process(Request *request, Response *response,
                               ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
    bool errorDetected = false;

    ResponseHeader *header = ResponseHeaderFactory().build();
    Response *resp = ResponseFactory().build(header);

    // Check for error and set status code accordingly
    if (errorDetected)
    {
        // Error detected, setting the response for a 404 Not Found as an example
        std::ostringstream statusLine;
        statusLine << "HTTP/1.1 404 Not Found\r\n";
        header->setStatusLine(statusLine.str());
    }
    else
    {
        // No error detected
        std::ostringstream statusLine;
        statusLine << "HTTP/1.1 200 OK\r\n";
        header->setStatusLine(statusLine.str());
    }

    // Example: Set the body of the response based on the scenario
    std::string responseBody = "The content of the response ...";

    char *bodyBinary = new char[responseBody.length() + 1]; // +1 for null terminator
    std::copy(responseBody.begin(), responseBody.end(), bodyBinary);
    bodyBinary[responseBody.length()] = '\0';

    resp->setBodyBin(bodyBinary);
    resp->setBodyLength(responseBody.length());

    return resp;
}

void FiltreError::setConfig(Config *conf)
{
    config = conf;
}

std::string FiltreError::toString()
{
    return "FiltreError";
}

void FiltreError::addProperty(std::string name, std::string value)
{
    config->addParam(name, value);
}

ProcessorTypeEnum FiltreError::getType()
{
    return type;
}
