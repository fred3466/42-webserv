#include "FiltreResponseMimeType.h"
#include <algorithm>

FiltreResponseMimeType::FiltreResponseMimeType(MimeTypeHelper &mimeTypeHelper)
    : mimeTypeHelper(mimeTypeHelper) {}

std::string FiltreResponseMimeType::getResponseMimeType(const std::string &filePath) const
{
    // Extract the file extension from the filePath
    std::string::size_type dotPos = filePath.rfind('.');
    if (dotPos == std::string::npos)
    {
        return "application/octet-stream"; // Default MIME type
    }

    std::string extension = filePath.substr(dotPos + 1);

    // Convert to lowercase to standardize
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    // Use the MimeTypeHelper to find the MIME type
    return mimeTypeHelper.findMimeType(extension);
}

// Modify the Response object to include the correct MIME type header
Response *FiltreResponseMimeType::process(Request *request, Response *response,
                                          ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
    ResponseHeader *header = ResponseHeaderFactory().build();
    Response *resp = ResponseFactory().build(header);

    std::string path;
    if (RequestHttp *httpReq = dynamic_cast<RequestHttp *>(request))
    {
        path = httpReq->getPath();
    }
    else
    {
        return resp;
    }

    // Extracting MIME type using the file path
    std::string mimeType = getResponseMimeType(path);

    // Add MIME type to the response header
    if (header)
    {
        std::string contentTypeHeader = "Content-Type: " + mimeType;
        header->addField(contentTypeHeader);
    }

    return response;
}

void FiltreResponseMimeType::setConfig(Config *conf)
{
    // Use the correct method to retrieve configuration parameters
    std::string mimeTypesFilePath = conf->getParamStr("mimeTypesFilePath", "example/mime.types");
    if (!mimeTypesFilePath.empty())
    {
        mimeTypeHelper.reloadMappingsFromFile(mimeTypesFilePath);
    }
}

std::string FiltreResponseMimeType::toString()
{
    std::ostringstream oss;
    oss << "FiltreResponseMimeType Processor with ";
    oss << mimeTypeHelper.numberOfMappings();
    oss << " MIME type mappings.";
    return oss.str(); // Convert the ostringstream to string and return it
}

// void FiltreResponseMimeType::addProperty(std::string name, std::string value)
// {
//     properties[name] = value;
// }

std::string FiltreResponseMimeType::getProperty(const std::string &name) const
{
    std::map<std::string, std::string>::const_iterator it = properties.find(name);
    if (it != properties.end())
    {
        return it->second;
    }
    return ""; // Return empty string if the property is not found
}

void FiltreResponseMimeType::addProperty(std::string name, std::string value)
{
    properties[name] = value;

    // If a specific property is updated, perform an action
    if (name == "configReloadTrigger" && value == "true")
    {
        // Perform the reload or reset action
        reloadConfigurations();
        // Optionally, reset the trigger
        properties[name] = "false";
    }
}

void FiltreResponseMimeType::reloadConfigurations()
{
    std::string mimeTypesFilePath = getProperty("example/mime.types");
    if (!mimeTypesFilePath.empty())
    {
        mimeTypeHelper.reloadMappingsFromFile(mimeTypesFilePath);
    }
}

// Response *FiltreResponseMimeType::process(Request *request, Response *response,
//                                           ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
// {
//     RequestHttp *httpReq = static_cast<RequestHttp *>(request);
//     ResponseHttp *httpResp = static_cast<ResponseHttp *>(response);

//     if (!httpReq || !httpResp)
//         return response;

//     std::string path = httpReq->getPath();

//     // Extracting MIME type using the file path
//     std::string mimeType = getResponseMimeType(path);

//     ResponseHeader *respHeader = httpResp->getHeader();
//     if (respHeader)
//     {
//         // Construct the Content-Type header string
//         std::string contentTypeHeader = "Content-Type: " + mimeType;
//         respHeader->addField(contentTypeHeader);
//     }

//     return response;
// }