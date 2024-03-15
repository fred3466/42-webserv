#include "CookieHelper.h"

CookieHelper::CookieHelper()
{
}

CookieHelper::~CookieHelper()
{
}

std::list<Cookie> CookieHelper::addCookie(const std::list<Cookie> &cookies, const Cookie &cookie)
{
	std::list<Cookie> newCookies = cookies;
	newCookies.push_back(cookie);
	return newCookies;
}

std::list<Cookie> CookieHelper::removeCookie(const std::list<Cookie> &cookies, const std::string &cookieName)
{
	std::list<Cookie> newCookies;
	for (std::list<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++)
	{
		if (it->getName() != cookieName)
		{
			newCookies.push_back(*it);
		}
	}
	return newCookies;
}

Cookie CookieHelper::getCookie(const std::list<Cookie> &cookies, const std::string &cookieName)
{
	for (std::list<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++)
	{
		if (it->getName() == cookieName)
		{
			return *it;
		}
	}
	return Cookie();
}

std::string CookieHelper::getCookieString(const std::list<Cookie> &cookies)
{
	std::string ret;
	for (std::list<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++)
	{
		ret += it->getName() + "=" + it->getValue() + "; ";
	}
	ret.erase(ret.end() - 2, ret.end());
	return ret;
}

std::string CookieHelper::getCookieStringResponse(const std::list<Cookie> &cookies)
{
	std::string ret;
	for (std::list<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++)
	{
		std::ostringstream oss;
    	oss << it->getMaxAge();
		ret += "Set-Cookie: ";
		ret += it->getName() + "=" + it->getValue() + "; ";
		ret += "Max-Age=" + oss.str() + "; "; 
		ret += "Domain=" + it->getDomain() + "; ";
		ret += "Path=" + it->getPath() + "\r\n";
	}
	return ret;
}