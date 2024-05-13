#pragma once
#include <string>

//#include "../FdPollfdServerTuple.h"

class Connector;
class FdPollfdServerTuple;

class ConnectorEvent
{
private:
	int *fdClient;
	std::string eventName;
	char *byteBuffer;
	int len;
	FdPollfdServerTuple *fdPollfdServerTuple;

public:
	ConnectorEvent(const ConnectorEvent &other);
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
//	ConnectorEvent(Request *req, Response *resp);
	ConnectorEvent(std::string eventName);
	~ConnectorEvent();
	void setByteBuffer(char *bytess);
	char* getByteBuffer();
	void setFdClient(int *fd);
	int* getFdClient();
	int getLen();
	void setLen(int len);
	std::string const& getEventName() const;
	FdPollfdServerTuple* getFdPollfdServerTuple();
	void setFdPollfdServerTuple(FdPollfdServerTuple *fdPollfdServerTuple);
};

