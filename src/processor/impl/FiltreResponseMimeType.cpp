#include "FiltreResponseMimeType.h"

#include <algorithm>

FiltreResponseMimeType::FiltreResponseMimeType(ProcessorTypeEnum type) : type(type), processorHelper()
{
	mimeTypeHelper = new MimeTypeHelper();
}

FiltreResponseMimeType::~FiltreResponseMimeType()
{
	delete mimeTypeHelper;
}

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
	return mimeTypeHelper->findMimeType(extension);
}

// Modify the Response object to include the correct MIME type header
Response* FiltreResponseMimeType::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void) processorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
	std::string path;
	path = request->getUri().getUri();

	// Extracting MIME type using the file path
	std::string mimeType = getResponseMimeType(path);

	// Add MIME type to the response header
	if (header)
	{
		header->addNoReplaceField("Content-Type", mimeType + ";");
//		header->addField("X-FiltreResponseMimeType-Content-Type", mimeType + ";");
	}

	return response;
}

void FiltreResponseMimeType::setConfig(Config *conf)
{
	// Use the correct method to retrieve configuration parameters
	this->config = conf;

	std::string mimeTypesFilePath = conf->getParamStr("mimeTypesFilePath", "example/mime.types");
	if (!mimeTypesFilePath.empty())
	{
		mimeTypeHelper->reloadMappingsFromFile(mimeTypesFilePath);
	}
}

std::string FiltreResponseMimeType::toString()
{
	std::ostringstream oss;
	oss << "FiltreResponseMimeType ";
	oss << +type;
	oss << " Processor with ";
	oss << mimeTypeHelper->numberOfMappings();
	oss << " MIME type mappings.";
	return oss.str(); // Convert the ostringstream to string and return it
}

// void FiltreResponseMimeType::addProperty(std::string name, std::string value)
// {
//     properties[name] = value;
// }

void FiltreResponseMimeType::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
//	properties[name] = value;
//
//	// If a specific property is updated, perform an action
//	if (name == "configReloadTrigger" && value == "true")
//	{
//		// Perform the reload or reset action
//		reloadConfigurations();
//		// Optionally, reset the trigger
//		properties[name] = "false";
//	}
}

//void FiltreResponseMimeType::reloadConfigurations()
//{
//	std::string mimeTypesFilePath = getProperty("exemple/mime.types");
//	if (!mimeTypesFilePath.empty())
//	{
//		mimeTypeHelper->reloadMappingsFromFile(mimeTypesFilePath);
//	}
//}

ProcessorTypeEnum FiltreResponseMimeType::getType()
{
	return type;
}

bool FiltreResponseMimeType::isExclusif()
{
	return false;
}

bool FiltreResponseMimeType::isBypassingExclusif()
{
	return false;
}

std::string FiltreResponseMimeType::getProperty(std::string name, std::string defaultVal)
{
	std::string val = config->getParamStr(name, defaultVal);
	return val;
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
