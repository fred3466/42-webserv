#pragma once
#include <string>

#include "../FdPollfdServerTuple.h"
#include "../API/Request.h"

class Connector;

class RequestResponseParam {
private:
	std::string eventName;
	char *byteBuffer;
	int len;
	FdPollfdServerTuple *fdPollfdServerTuple;
	Request *request;

public:
	RequestResponseParam();
	RequestResponseParam(const RequestResponseParam &other);
	RequestResponseParam(std::string eventName);
	~RequestResponseParam();
	void setByteBuffer(char *bytess);
	char* getByteBuffer();
	void setFdClient(int fd);
	int getFdClient();
	int getLen();
	void setLen(int len);
	std::string const& getEventName() const;
	FdPollfdServerTuple* getFdPollfdServerTuple();
	void setFdPollfdServerTuple(FdPollfdServerTuple *fdPollfdServerTuple);
	Request* getRequest();
	void setRequest(Request *request);
};

