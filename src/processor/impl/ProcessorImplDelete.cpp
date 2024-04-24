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
    harl.debug("Received DELETE request for URI: " + request->getUri().getUri());
    std::string fileName = request->getUrlParam("file");
    // std::string base_path = config->getParamStr("base_path", "base_path_missing");
    // std::string fullPath = FileUtil().realPathFile(base_path + "/" + fileName);
    std::string base_path = config->getParamStr("base_path", "/");
    std::string fullPath = FileUtil().realPathFile(base_path + fileName);

    harl.debug("Full path resolved to: " + fullPath);

    std::ifstream file(fullPath.c_str());

    if (!file.good())
    {
        harl.info("File not found at path: " + fullPath);
        response->setErrorCodeTmp(404);
        return response;
    }
    // file.close();

    if (std::remove(fullPath.c_str()) != 0)
    {
        harl.error("Failed to delete file at path: " + fullPath);
        response->setErrorCodeTmp(500);
        return response;
    }
    else
    {
        harl.info("Successfully deleted file at path: " + fullPath);
        response->setErrorCodeTmp(204);
        return response;
    }
}

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
