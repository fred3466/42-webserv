#include "HttpError.h"

HttpError::HttpError(/* args */)
{
}

HttpError::~HttpError()
{
}

ResponseHttp* HttpError::generateErrorResponse(int errorCode, const std::string &errorMessage)
{
	std::string errorPageContent = loadErrorPageTemplate();         // Load HTML template
	replacePlaceholders(errorPageContent, errorCode, errorMessage); // Replace placeholders

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

std::string HttpError::loadErrorPageTemplate()
{
	std::ifstream file("htdocs/error_404.html");
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

void HttpError::replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage)
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
