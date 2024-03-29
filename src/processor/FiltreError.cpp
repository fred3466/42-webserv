#include "FiltreError.h"

FiltreError::FiltreError(ProcessorTypeEnum type) : Processor(), type(type)
{
}

FiltreError::~FiltreError()
{
}

Response *FiltreError::process(Request * /*request*/, Response *response,
                               ProcessorAndLocationToProcessor * /*processorAndLocationToProcessor*/)
{
    int errorCode = response->getErrorCodeTmp();

    if (errorCode != 200)
    {
        HttpError *error = HttpErrorFactory::build(errorCode);
        HttReturnCodeHelper returnCodeHelper;

        std::string statusLine = returnCodeHelper.getStatusLine(errorCode);

        std::string errorPageContent = returnCodeHelper.loadErrorPageTemplate();
        replacePlaceholders(errorPageContent, errorCode, error->getDescription());

        response->setStatusLine(statusLine);

        char *bodyBinary = new char[errorPageContent.length() + 1];
        std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBinary);
        bodyBinary[errorPageContent.length()] = '\0';

        response->setBodyBin(bodyBinary);
        response->setBodyLength(errorPageContent.length());

        delete error;
    }
    else
    {
        // Case where there is no error
    }

    return response;
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

Response *FiltreError::generateErrorResponse(int errorCode, const std::string &errorMessage)
{
    std::string errorPageContent = loadErrorPageTemplate();
    replacePlaceholders(errorPageContent, errorCode, errorMessage);

    // Create a new response header and set the status line appropriately
    ResponseHeader *header = new ResponseHttpHeader();
    std::ostringstream statusLine;
    statusLine << "HTTP/1.1 " << errorCode << " " << errorMessage;
    header->setStatusLine(statusLine.str());
    header->addField("Content-Type", "text/html");

    // Create the response object and set the body and header
    //	TODO utilise la factory STP
    ResponseHttp *response = new ResponseHttp(header);
    char *bodyBin = new char[errorPageContent.length() + 1];
    std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBin);
    bodyBin[errorPageContent.length()] = '\0'; // Null-terminate the string
    response->setBodyBin(bodyBin);
    response->setBodyLength(errorPageContent.length());

    return response;
}

std::string FiltreError::loadErrorPageTemplate()
{
    std::ifstream file("htdocs/error_404.html");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

void FiltreError::replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage)
{
    // Replace [HTTP_ERROR_CODE] and [HTTP_ERROR_DESC] in the content string
    std::stringstream codeStream;
    codeStream << errorCode;
    std::string codeStr = codeStream.str();

    size_t pos;
    while ((pos = content.find("[HTTP_ERROR_CODE]")) != std::string::npos)
    {
        content.replace(pos, std::string("[HTTP_ERROR_CODE]").length(), codeStr);
    }
    while ((pos = content.find("[HTTP_ERROR_DESC]")) != std::string::npos)
    {
        content.replace(pos, std::string("[HTTP_ERROR_DESC]").length(), errorMessage);
    }
}

// Response *FiltreError::process(Request * /*request*/, Response *response,
//                                ProcessorAndLocationToProcessor * /*processorAndLocationToProcessor*/)
// {
//     int errorCode = response->getErrorCodeTmp(); // This needs to be determined based on your application logic

//     // Generate the error response only if there is an error code set
//     if (errorCode != 200)
//     {
//         HttpError *error = HttpErrorFactory::build(errorCode);
//         HttReturnCodeHelper returnCodeHelper;

//         // Generate the status line for the error code
//         std::string statusLine = returnCodeHelper.getStatusLine(errorCode);

//         // Load and customize the error page content
//         std::string errorPageContent = returnCodeHelper.loadErrorPageTemplate();
//         replacePlaceholders(errorPageContent, errorCode, error->getDescription());

//         // Attempt to cast the response object to ResponseHttp to access specific methods
//         ResponseHttp *httpResp = dynamic_cast<ResponseHttp *>(response);
//         if (httpResp)
//         {
//             // If casting is successful, modify the status line of the response's header
//             ResponseHttpHeader *httpHeader = dynamic_cast<ResponseHttpHeader *>(httpResp->getHeader());
//             if (httpHeader)
//             {
//                 httpHeader->setStatusLine(statusLine);
//             }

//             // Set up the body of the response
//             char *bodyBinary = new char[errorPageContent.length() + 1];
//             std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBinary);
//             bodyBinary[errorPageContent.length()] = '\0';

//             httpResp->setBodyBin(bodyBinary);
//             httpResp->setBodyLength(errorPageContent.length());
//         }
//         delete error;
//     }
//     else
//     {
//         // Handle the case where there is no error
//     }

//     return response;
// }
