#include "HttpReturnCodeHelper.h"

HttpReturnCodeHelper::HttpReturnCodeHelper() : httpReturnCode(200)
{
}

HttpReturnCodeHelper::~HttpReturnCodeHelper()
{
}

HttpReturnCodeHelper::HttpReturnCodeHelper(int httpReturnCode)
{
	this->httpReturnCode = httpReturnCode;
}

//std::string HttpReturnCodeHelper::getStatusLine()
//{
//	ResponseTools responseTools;
//	std::string reasonPhrase = responseTools.buildStatusLineForCode(httpReturnCode);
//
//	std::ostringstream ssHttpReturnCode;
//	ssHttpReturnCode << httpReturnCode;
//	std::string statusLine = "HTTP/1.1 " + ssHttpReturnCode.str() + " " + reasonPhrase + "\r\n";
//	return statusLine;
//}

void HttpReturnCodeHelper::replacePlaceholders(std::string &content, const std::string &errorMessage)
{
	// Replace [HTTP_ERROR_CODE] and [HTTP_ERROR_DESC] in the content string
	std::stringstream codeStream;
	codeStream << this->httpReturnCode;
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

/*
 HttReturnCodeHelper helper;
 std::string statusLine;

 statusLine = helper.getStatusLine(200);

 */
