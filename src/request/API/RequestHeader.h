#pragma once
#include <string>
#include <list>
#include "../../cookie/Cookie.h"
#include "../../Uri/Uri.h"

class RequestHeader
{
private:

public:
	RequestHeader();
	virtual ~RequestHeader();

	virtual void addField(std::string f)=0;
	virtual const std::list<std::string>& getFields() const =0;
	virtual std::string getFieldValue(std::string fieldName) const =0;
	virtual const std::string& getMethod() const =0;
	virtual void setMethod(const std::string &method)=0;
	virtual const std::string& getUri() const =0;
	virtual void setUri(const std::string &uri)=0;
	virtual const std::string& getVersion() const =0;
	virtual void setVersion(const std::string &version)=0;
	virtual std::string toString()=0;
	virtual Cookie getCookie(const std::string &cookieName)=0;
	virtual bool addCookie(const Cookie &cookie)=0;
	virtual bool removeCookie(const std::string &cookieName)=0;
	virtual std::string getCookieString()=0;
	virtual const std::string& getQueryString() const =0;
	virtual const std::string getFileExtension() const =0;
	virtual const std::string getFileName() const =0;
	virtual const std::string& getPath() const=0;
};
