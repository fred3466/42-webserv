#pragma once

#include "../util/StringUtil.h"
#include "../cookie/CookieHelper.h"
#include "../cookie/factory/CookieFactory.h"
#include "../Uri/Uri.h"
#include <list>
#include <map>
#include "../API/Request.h"
#include "../API/RequestHeader.h"

class RequestHttpHeader: public RequestHeader {
private:
	std::list<std::string> fields;
	std::list<Cookie> cookies;
	CookieHelper cookieHelper;
	std::string method;
	Uri uri;
	std::string version;
	int headerSize;

public:
	RequestHttpHeader(char *rawRequest, int const rawRequestLen);
	virtual ~RequestHttpHeader();

	virtual void addField(std::string f);
	virtual const std::list<std::string>& getFields() const;
	virtual std::string getFieldValue(std::string fieldName) const;
	virtual const std::string& getMethod() const;
	virtual void setMethod(const std::string &method);
	virtual const Uri& getUri() const;
	virtual void setUri(const Uri &uri);
	virtual const std::string& getVersion() const;
	virtual void setVersion(const std::string &version);
	virtual std::string toString();
	virtual Cookie getCookie(const std::string &cookieName);
	virtual bool addCookie(const Cookie &cookie);
	virtual bool removeCookie(const std::string &cookieName);
	virtual void clearCookies();
	virtual std::string getCookieString();
	virtual const std::string& getQueryString() const;
	virtual const std::string getFileExtension() const;
	virtual const std::string getFileName() const;
	virtual const std::string& getPath() const;
	virtual size_t getHeaderSize() const;
	virtual void setHeaderSize(size_t headerSize);
};
