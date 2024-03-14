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