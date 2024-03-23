#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include "../util/StringUtil.h"
#include <map>
#include <iterator>
#include <string>

class RequestHttp: public Request
{
private:
	RequestHeader *header;
	int *fdClient;

public:
	RequestHttp();
	~RequestHttp();
	RequestHttp(RequestHeader *header);

	virtual const std::list<std::string>& getFields() const;
	virtual std::string getHeaderFieldValue(std::string fieldName) const;
	virtual void addField(std::string rawField) const;
	virtual const std::string& getUri() const;
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

	void setBody(const std::string &b);
	const std::string& getBody() const;

	virtual bool isConnectionKeepAlive() throw (char*);
};
