#include "ResponseHttpHeader.h"

ResponseHttpHeader::ResponseHttpHeader() :
		fields()
{
}

ResponseHttpHeader::~ResponseHttpHeader()
{
}

void ResponseHttpHeader::addField(std::string f)
{
	if (!f.empty())
		fields.push_back(f);
}
const std::list<std::string>& ResponseHttpHeader::getFields() const
{
	return fields;
}

const std::string& ResponseHttpHeader::getStatusLine() const
{
	return statusLine;
}

void ResponseHttpHeader::setStatusLine(const std::string &statusLine)
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
	return cookieHelper.getCookieString(cookies);
}