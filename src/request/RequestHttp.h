#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include "API/RequestBody.h"
#include "../util/StringUtil.h"
#include <map>
#include <iterator>
#include <string>

//TODO En faire une Coplien
class RequestHttp: public Request
{
private:
	RequestHeader *header;
	RequestBody *body;
	int *fdClient;
	std::string host;
	int port;

public:
	RequestHttp();
	~RequestHttp();
	RequestHttp(RequestHeader *header, RequestBody *body);

	virtual const std::list<std::string>& getFields() const;
	virtual std::string getHeaderFieldValue(std::string fieldName) const;
	virtual void addField(std::string rawField) const;
	virtual const Uri& getUri() const;
	virtual const std::string& getMethod() const;
	//	virtual void dump() const;
	virtual void setFdClient(int *fd);
	virtual int* getFdClient() const;
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
};
