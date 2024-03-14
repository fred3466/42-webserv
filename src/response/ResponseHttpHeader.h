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
	virtual const std::list<std::string>& getFields() const;
	virtual const std::string& getStatusLine() const;
	virtual void setStatusLine(const std::string &statusLine);
	virtual Cookie getCookie(const std::string &cookieName);
	virtual bool addCookie(const Cookie &cookie);
	virtual bool removeCookie(const std::string &cookieName);
};

