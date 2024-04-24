#include "ProcessorImplDelete.h"

// ProcessorImplDelete::ProcessorImplDelete() : Processor(HEADER_MODIFIER)
// {
// }

ProcessorImplDelete::ProcessorImplDelete(ProcessorTypeEnum type) : harl()
{
    this->type = type;
}

ProcessorImplDelete::~ProcessorImplDelete()
{
}

Response *ProcessorImplDelete::process(Request *request, Response *response,
                                       ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
    (void)processorAndLocationToProcessor;
    harl.debug("ProcessorImplDelete::process - Starting DELETE process for URI: " + request->getUri().getUri());

    std::string fileName = request->getUrlParam("file");
    //    std::string fullPath = "/htdocs" + fileName;
    std::string base_path = config->getParamStr("base_path", "base_path_missing");
    std::string fullPath = FileUtil().realPathFile(base_path + "/" + fileName);

    // std::string path = request->getUri().getUri();
    // std::ifstream file(path.c_str());

    std::ifstream file(fullPath.c_str());
    std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;

    if (!file.good())
    {
        // file.close();
        harl.info("ProcessorImplDelete::process - File not found: " + fullPath);
        response->setErrorCodeTmp(404);
        return response;
    }

    // file.close();

    if (std::remove(fullPath.c_str()) != 0)
    {
        harl.error("ProcessorImplDelete::process - Error deleting file: " + fullPath);
        response->setErrorCodeTmp(500);
        return response;
    }
    else
    {
        harl.info("ProcessorImplDelete::process - Successfully deleted file: " + fullPath);
        response->setErrorCodeTmp(204);
        return response;
    }
}

// Response *ProcessorImplDelete::handleHttpError(int errorCode, Response *response, HttpErrorFactory &errorFactory)
// {
//     HttpError *error = errorFactory.build(errorCode);
//     response->setStatusLine(error->getStatusLine());

//     harl.debug("ProcessorImplDelete::handleHttpError - Sending Status Line: " + error->getStatusLine());

//     if (errorCode != 204)
//     {
//         std::string content = error->getDescription();
//         harl.info("ProcessorImplDelete::handleHttpError - Sending Body Content: " + content);

//         char *bodyBin = new char[content.length() + 1];
//         std::copy(content.begin(), content.end(), bodyBin);
//         bodyBin[content.length()] = '\0';
//         response->setBodyBin(bodyBin);
//         response->setBodyLength(static_cast<int>(content.length()));
//     }
//     else
//     {
//         response->setBodyBin(NULL);
//         response->setBodyLength(0);
//         harl.debug("ProcessorImplDelete::handleHttpError - No Content for 204 Response");
//     }

//     return response;
// }

void ProcessorImplDelete::setConfig(Config *conf)
{
    config = conf;
}

std::string ProcessorImplDelete::toString()
{
    return "ProcessorImplDelete";
}

void ProcessorImplDelete::addProperty(std::string name, std::string value)
{
    config->addParam(name, value);
}

std::string ProcessorImplDelete::getProperty(std::string name, std::string defaultVal)
{
    std::string val = config->getParamStr(name, defaultVal);
    return val;
}

ProcessorTypeEnum ProcessorImplDelete::getType()
{
    return type;
}

bool ProcessorImplDelete::isExclusif()
{
    return false;
}

bool ProcessorImplDelete::isBypassingExclusif()
{
    return false;
}

// Response *ProcessorImplDelete::process(Request *request, Response *response, HttpErrorFactory &errorFactory)
// {
//     std::string path = request->getUri().getUri();
//     std::ifstream file(path.c_str());

//     // File does not exist
//     if (!file.good())
//     {
//         file.close();
//         return handleHttpError(404, response, errorFactory);
//     }

//     file.close();

//     // Attempt to delete the file
//     if (std::remove(path.c_str()) != 0)
//     {
//         return handleHttpError(500, response, errorFactory);
//     }
//     else
//     {
//         return handleHttpError(204, response, errorFactory); // Successfully deleted, No Content
//     }
// }

// Response *ProcessorImplDelete::handleHttpError(int errorCode, Response *response, HttpErrorFactory &errorFactory)
// {
//     HttpError *error = errorFactory.build(errorCode);

//     response->setStatusLine(error->getStatusLine());

//     // Only set content if it's not a 204 No Content response
//     if (errorCode != 204)
//     {
//         std::string content = error->getDescription(); // Use description from the error object
//         char *bodyBin = new char[content.length() + 1];
//         std::copy(content.begin(), content.end(), bodyBin);
//         bodyBin[content.length()] = '\0'; // Null-terminate the string
//         response->setBodyBin(bodyBin);
//         response->setBodyLength(static_cast<int>(content.length()));
//     }
//     else
//     {
//         response->setBodyBin(NULL);
//         response->setBodyLength(0);
//     }

//     return response;
// }

/*for test
#include <sstream> // Include this if it's not already included

Response *ProcessorImplDelete::handleHttpError(int errorCode, Response *response, HttpErrorFactory &errorFactory)
{
    HttpError *error = errorFactory.build(errorCode);

    // Using std::ostringstream to convert integer to string
    std::ostringstream ss;
    ss << errorCode;
    std::string errorCodeStr = ss.str();

    harl.debug("ProcessorImplDelete::handleHttpError - Setting response status line for error code: " + errorCodeStr);
    response->setStatusLine(error->getStatusLine());

    if (errorCode != 204)
    {
        std::string content = error->getDescription();
        char *bodyBin = new char[content.length() + 1];
        std::copy(content.begin(), content.end(), bodyBin);
        bodyBin[content.length()] = '\0';
        response->setBodyBin(bodyBin);
        response->setBodyLength(static_cast<int>(content.length()));
        harl.debug("ProcessorImplDelete::handleHttpError - Response body set for error: " + content);
    }
    else
    {
        response->setBodyBin(NULL);
        response->setBodyLength(0);
        harl.debug("ProcessorImplDelete::handleHttpError - No content response set for 204");
    }

    return response;
}

*/