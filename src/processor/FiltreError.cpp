#include "FiltreError.h"
#include "../error/HttpReturnCodeHelper.h"

FiltreError::FiltreError(ProcessorTypeEnum type) : Processor(), type(type), config()
{
}

FiltreError::~FiltreError()
{
}

Response *FiltreError::process(Request *request, Response *response,
							   ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void)request;
	(void)processorAndLocationToProcessor;

	int errorCode = response->getErrorCodeTmp();

	HttpError *error = HttpErrorFactory().build(errorCode);
	response->setHttpError(error);

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

bool FiltreError::isExclusif()
{
	return false;
}

bool FiltreError::isBypassingExclusif()
{
	return true;
}

// Response *FiltreError::process(Request *request, Response *response,
// 							   ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
// {
// 	(void)request;
// 	(void)processorAndLocationToProcessor;

// 	int errorCode = response->getErrorCodeTmp();

// 	HttpError *error = HttpErrorFactory().build(errorCode);
// 	response->setHttpError(error);

// 	if (errorCode != 200)
// 	{
// 		HttpError *error = response->getHttpError();
// 		HttpReturnCodeHelper returnCodeHelper;

// 		std::string statusLine = returnCodeHelper.getStatusLine(errorCode);

// 		std::string errorPageContent = returnCodeHelper.loadErrorPageTemplate();
// 		replacePlaceholders(errorPageContent, errorCode, error->getDescription());

// 		response->setStatusLine(statusLine);

// 		char *bodyBinary = new char[errorPageContent.length() + 1];
// 		std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBinary);
// 		bodyBinary[errorPageContent.length()] = '\0';

// 		response->setBodyBin(bodyBinary);
// 		response->setBodyLength(errorPageContent.length());

// 		delete error;
// 	}
// 	else
// 	{
// 		// Case where there is no error
// 	}

// 	return response;
// }