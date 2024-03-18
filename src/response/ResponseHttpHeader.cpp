#include "ResponseHttpHeader.h"

ResponseHttpHeader::ResponseHttpHeader() :
		fields(), su()
{
}

ResponseHttpHeader::~ResponseHttpHeader()
{
}

void ResponseHttpHeader::addField(std::string headerName, std::string headerValue)
{
	std::string keyValue = su.trim(headerName) + ": " + su.trim(headerValue) + "\r\n";
	if (!keyValue.empty())
		fields.push_back(keyValue);
}
std::list<std::string> ResponseHttpHeader::getFields()
{
	return fields;
}

std::string ResponseHttpHeader::getStatusLine()
{
	return statusLine;
}

void ResponseHttpHeader::setStatusLine(std::string statusLine)
{
	this->statusLine = statusLine;
}

Cookie ResponseHttpHeader::getCookie(const std::string &cookieName)
{
	return cookieHelper.getCookie(cookies, cookieName);
}

bool ResponseHttpHeader::addCookie(const Cookie &cookie)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.addCookie(cookies, cookie);
	if (cookies.size() > i)
		ret = true;
	return ret;
}

bool ResponseHttpHeader::removeCookie(const std::string &cookieName)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.removeCookie(cookies, cookieName);
	if (cookies.size() < i)
		ret = true;
	return ret;
}

std::string ResponseHttpHeader::getCookieString()
{
	return cookieHelper.getCookieStringResponse(cookies);
}
