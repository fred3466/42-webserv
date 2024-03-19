#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head) : fdClient(-1)
{
	header = head;
}

RequestHeader* RequestHttp::getHeader() const
{
	return header;
}

std::string RequestHttp::getFileExtension() const
{
	StringUtil su = StringUtil();
	std::string uri = getUri();
	std::string fileExt = uri.substr(uri.rfind(".", std::string::npos));
	if (!fileExt.empty())
	{
		return su.strUpperCase(fileExt);
	}
	return "";
}

std::string RequestHttp::getFileName() const
{
	std::string uri = getUri();
	std::string fileName = uri.substr(uri.rfind("/") + 1, std::string::npos);
	return fileName;
}

//https://httpd.apache.org/docs/current/vhosts/name-based.html
//std::string RequestHttp::getHost() const
//{
//	std::string uri = getUri();
//	std::string host = ""; // Initialize host to an empty string
//
//	// Find the position of "://"
//	size_t schemeEndPos = uri.find("://");
//	if (schemeEndPos != std::string::npos)
//	{
//		// Find the start of the host by skipping the "://"
//		size_t hostStart = schemeEndPos + 3;
//
//		// Find the end of the host, which could be the start of the path or query string
//		size_t hostEnd = uri.find('/', hostStart);
//		size_t queryStart = uri.find('?', hostStart);
//
//		// stop at the earliest of the path or query string
//		if (hostEnd == std::string::npos)
//			hostEnd = queryStart;
//		if (queryStart != std::string::npos && queryStart < hostEnd)
//			hostEnd = queryStart;
//
//		// If there's no path or query string, the host runs till the end of the URI
//		if (hostEnd == std::string::npos)
//			hostEnd = uri.length();
//
//		// Extract the host
//		host = uri.substr(hostStart, hostEnd - hostStart);
//	}
//	return host;
//}
std::string RequestHttp::getHost() const
{
	std::string uri = getUri();

	size_t posFirstSlash = uri.find("/");
	size_t posSecondSlash = uri.find("/", posFirstSlash + 1);
//	TODO à améliorer
	size_t posFirstColon = uri.find(":");
	size_t endIndex = ((posFirstColon != std::string::npos) && posFirstColon < posSecondSlash ? posFirstColon : posSecondSlash);
//	TODO avec ou sans  / ?
	std::string path = uri.substr(posFirstSlash, endIndex - posFirstSlash + 1);
	return path;
}
//https://httpd.apache.org/docs/current/vhosts/name-based.html
//std::string RequestHttp::getPath() const
//{
//	std::string uri = getUri();
////	TODO Pas vraiment un URL ou un URI...
//	size_t posFirstSlash = uri.find("/");
//	size_t posLastSlash = uri.rfind("/");
////	TODO avec ou sans  / ?
//	std::string path = uri.substr(posFirstSlash, posLastSlash - posFirstSlash + 1);
//	return path;
//}

std::string RequestHttp::getPath() const
{
	std::string uri = header->getUri();
	size_t queryPos = uri.find('?');
	if (queryPos != std::string::npos)
	{
		return uri.substr(0, queryPos); // Return the URI path without the query string
	}
	return uri; // Return the full URI if there is no query string
}

std::string RequestHttp::getQueryString() const
{
	std::string queryString = ""; // Initialize queryString to an empty string
	std::string uri = getUri();
	size_t queryPos = uri.find('?');
	if (queryPos != std::string::npos)
	{
		queryString = uri.substr(queryPos + 1);
	}
	return queryString;
}

std::string RequestHttp::getHeaderFieldValue(std::string fieldName) const
{
	std::string fieldValue = header->getFieldValue(fieldName);
	return fieldValue;
}

void RequestHttp::addField(std::string rawField) const
{
	getHeader()->addField(rawField);
}

const std::list<std::string>& RequestHttp::getFields() const
{
	return header->getFields();
}

std::string RequestHttp::getUri() const
{
	return header->getUri();
}
std::string RequestHttp::getMethod() const
{
	return header->getMethod();
}

int RequestHttp::getFdClient() const
{
	return fdClient;
}
void RequestHttp::setFdClient(int fd)
{
	fdClient = fd;
}
