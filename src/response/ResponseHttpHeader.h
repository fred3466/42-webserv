#pragma once
#include <string>
#include <list>
#include "API/ResponseHeader.h"
#include "../cookie/Cookie.h"
#include "../cookie/CookieHelper.h"

class ResponseHttpHeader: public ResponseHeader
{
private:
	std::string statusLine;
	std::list<std::string> fields;
	std::list<Cookie> cookies;
	CookieHelper cookieHelper;

public:
	ResponseHttpHeader();
	virtual ~ResponseHttpHeader();

	virtual void addField(std::string f);
	virtual std::list<std::string> getFields();
	virtual std::string getStatusLine();
	virtual void setStatusLine(std::string statusLine);
	virtual Cookie getCookie(const std::string &cookieName);
	virtual bool addCookie(const Cookie &cookie);
	virtual bool removeCookie(const std::string &cookieName);
	virtual std::string getCookieString();
};

