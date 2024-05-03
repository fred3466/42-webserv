#pragma once

#include "../connector/ConnectorEvent.h"
//#include "../request/API/Request.h"
//#include "../response/API/Response.h"

class ConnectorListener
{
private:

protected:

public:
	ConnectorListener();
	virtual ~ConnectorListener();
	virtual void onIncomming(ConnectorEvent *e)=0;
	virtual void onDataReceiving(ConnectorEvent *e)=0;
//	virtual bool operator==(const ConnectorListener &other)=0;

};

