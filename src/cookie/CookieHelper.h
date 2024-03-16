#pragma once

#include "Cookie.h"
#include <list>

class CookieHelper
{
private:
public:
	CookieHelper();
	~CookieHelper();
	Cookie getCookie(const std::list<Cookie> &cookies, const std::string &cookieName);
	std::list<Cookie> addCookie(const std::list<Cookie> &cookies, const Cookie &cookie);
	std::list<Cookie> removeCookie(const std::list<Cookie> &cookies, const std::string &cookieName);
	std::string getCookieString(const std::list<Cookie> &cookies);
	std::string getCookieStringResponse(const std::list<Cookie> &cookies);
};