#pragma once
#include <string>
#include <map>
#include <string>
#include <iostream>
#include "../API/RequestHeader.h"
#include <sstream>

class Request
{
private:
	public:
	Request();
	Request(RequestHeader *header);
	virtual ~Request();

	virtual const std::list<std::string>& getFields() const =0;
	virtual std::string getUri() const =0;
	virtual std::string getMethod() const =0;
	virtual std::string getHeaderFieldValue(std::string fieldName) const =0;
	//	virtual void dump() const =0;
	virtual void setFdClient(int fd)=0;
	virtual int getFdClient() const =0;
	virtual void addField(std::string rawField) const =0;
	virtual RequestHeader* getHeader() const =0;

	virtual std::string getQueryString() const =0;
	virtual std::string getFileExtension() const =0;
	virtual std::string getFileName() const =0;
	virtual std::string getPath() =0;
	virtual std::string getHost() =0;
};

