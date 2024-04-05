#include "FiltreRequestPost.h"

FiltreRequestPost::FiltreRequestPost(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil()
{
	this->type = type;
}

FiltreRequestPost::~FiltreRequestPost()
{
}

void FiltreRequestPost::setConfig(Config *conf)
{
	config = conf;
}

ProcessorTypeEnum FiltreRequestPost::getType()
{
	return type;
}

std::string FiltreRequestPost::toString()
{
	return "FiltreRequestPost " + type;
}
// TODO http error
/*
 If CONTENT_TYPE is blank, the script can reject the request
 with a 415 'Unsupported Media Type' error, where supported by the
 protocol.

 When parsing PATH_INFO, PATH_TRANSLATED or SCRIPT_NAME the script
 should be careful of void path segments ("//") and special path
 segments ("." and "..").  They should either be removed from the path
 before use in OS system calls, or the request should be rejected with
 404 'Not Found'.
 */
Response *FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void)processorAndLocationToProcessor;
	RequestBody *body = request->getBody();
	(void)body;
	//	response->set
	std::string CONTENT_TYPE = request->getHeaderFieldValue("Content-Type");
	if (CONTENT_TYPE == "application/x-www-form-urlencoded" || CONTENT_TYPE == "multipart/form-data")
	{
		//		POST POST POST POST POST POST POST POST POST POST POST

		std::string CONTENT_LENGTH = request->getHeaderFieldValue("Content-Length");
	}

	return response;
}

// Response *FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
// {
// 	(void)processorAndLocationToProcessor;
// 	RequestBody *body = request->getBody();
// 	(void)body;

// 	std::string CONTENT_TYPE = request->getHeaderFieldValue("Content-Type");
// 	std::string CONTENT_LENGTH = request->getHeaderFieldValue("Content-Length");

// 	// Debug: Print the Content-Type and Content-Length
// 	std::cout << "Debug: CONTENT_TYPE = " << CONTENT_TYPE << std::endl;
// 	std::cout << "Debug: CONTENT_LENGTH = " << CONTENT_LENGTH << std::endl;

// 	// Convert CONTENT_LENGTH to integer
// 	int contentLength = StringUtil().intFromStr(CONTENT_LENGTH);

// 	// Debug: Print the parsed content length
// 	std::cout << "Debug: Parsed contentLength = " << contentLength << std::endl;

// 	// Retrieve the maximum body size from the configuration
// 	int maxBodySize = config->getParamInt("max_body_size", -1); // Default to -1 if not set

// 	// Debug: Print the max body size
// 	std::cout << "Debug: maxBodySize from config = " << maxBodySize << std::endl;

// 	// Check if the content length exceeds the maximum body size
// 	if (maxBodySize != -1 && contentLength > maxBodySize)
// 	{
// 		// Debug: Print a message if the content length exceeds max body size
// 		std::cout << "Debug: Content length exceeds max body size, rejecting request." << std::endl;

// 		// Content length exceeds maximum, reject the request
// 		HttpError *error = HttpErrorFactory().build(413); // 413 Payload Too Large
// 		response->setHttpError(error);

// 		// Debug: Print that we're setting a 413 error
// 		std::cout << "Debug: Setting 413 Payload Too Large error in response." << std::endl;
// 		return response;
// 	}

// 	// Debug: If request is processed without exceeding max body size
// 	std::cout << "Debug: Request processed successfully without exceeding max body size." << std::endl;

// 	return response;
// }

void FiltreRequestPost::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}
