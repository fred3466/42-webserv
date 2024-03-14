#pragma once
#include <string>
#include <list>
#include "../../cookie/Cookie.h"

class ResponseHeader
{
private:

public:
	ResponseHeader();
	virtual ~ResponseHeader();

	virtual void addField(std::string f)=0;
	virtual const std::list<std::string>& getFields() const =0;
	virtual const std::string& getStatusLine() const =0;
	virtual void setStatusLine(const std::string &statusLine)=0;
	virtual Cookie getCookie(const std::string &cookieName)=0;
	virtual bool addCookie(const Cookie &cookie)=0;
	virtual bool removeCookie(const std::string &cookieName)=0;
};

