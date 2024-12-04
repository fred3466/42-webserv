#pragma once

#include "../util/StringUtil.h"
#include <map>
#include <iterator>
#include <string>
#include "../API/Request.h"
#include "../API/RequestBody.h"
#include "../API/RequestHeader.h"

class RequestHttp: public Request {
private:
	RequestHeader *header;
	RequestBody *body;
	int fdClient;
	std::string host;
	int port;
	std::string remoteIpPort;
	bool bConnectionKeepAlive;
	bool bResponseAlreadySent;

public:
	RequestHttp();
	~RequestHttp();
	RequestHttp(RequestHeader *header, RequestBody *body);

	virtual const std::list<std::string>& getFields() const;
	virtual std::string getHeaderFieldValue(std::string fieldName) const;
	virtual void addField(std::string rawField) const;
	virtual const Uri& getUri() const;
	virtual const std::string& getMethod() const;
	virtual void setFdClient(int fd);
	virtual int getFdClient() const;
	virtual const std::string& getQueryString() const;
	virtual RequestHeader* getHeader() const;
	virtual const std::string getFileExtension() const;
	virtual const std::string getFileName() const;
	virtual const std::string& getPath();
	virtual const std::string getHost();
	virtual int getPort();

	virtual void setBody(RequestBody *body);
	virtual RequestBody* getBody();

	virtual bool isConnectionKeepAlive() throw (char*);
	virtual std::string getRemoteIpPort();
	virtual std::string getUrlParam(std::string paramName);
	virtual bool isResponseAlreadySent();
	virtual void setResponseAlreadySent(bool bResponseAlreadySent);

};
