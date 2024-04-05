#pragma once
#include <string>
#include <map>
#include <string>
#include <iostream>
#include "../API/RequestHeader.h"
#include "../API/RequestBody.h"
#include "../../Uri/Uri.h"
#include <sstream>

class Request
{
private:
	public:
	Request();
	Request(RequestHeader *header, RequestBody *body);
	virtual ~Request();

	virtual const std::list<std::string>& getFields() const =0;
	virtual const Uri& getUri() const =0;
	virtual const std::string& getMethod() const =0;
	virtual std::string getHeaderFieldValue(std::string fieldName) const =0;
	virtual void setFdClient(int *fd)=0;
	virtual int* getFdClient() const =0;
	virtual void addField(std::string rawField) const =0;
	virtual RequestHeader* getHeader() const =0;

	virtual const std::string& getQueryString() const =0;
	virtual const std::string getFileExtension() const =0;
	virtual const std::string getFileName() const =0;
	virtual const std::string& getPath() =0;
	virtual const std::string getHost() =0;
	virtual int getPort()=0;

	virtual bool isConnectionKeepAlive() throw (char*)=0;
	virtual void setBody(RequestBody *body)=0;
	virtual RequestBody* getBody() =0;
};

