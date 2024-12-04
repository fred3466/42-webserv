#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

#include "../config.h"
#include "../API/RequestBody.h"

class RequestHttpBody: public RequestBody {
private:
	char *content;
	int len;

public:
	RequestHttpBody(char *rawRequest, int rawRequestLen, int headerSize);
	virtual ~RequestHttpBody();
	RequestHttpBody(const RequestHttpBody &other);
	RequestHttpBody& operator=(const RequestHttpBody &other);

	virtual char* getContent();
	virtual int getLen();
	void setLen(int len);
};

