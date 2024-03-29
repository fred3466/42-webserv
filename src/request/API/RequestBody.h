#pragma once
#include <string>

class RequestBody
{
public:
	RequestBody();
	virtual ~RequestBody();
	RequestBody(const RequestBody &other);
	RequestBody& operator=(const RequestBody &other);
	virtual std::string* getContent()=0;
};
