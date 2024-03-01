#pragma once

#include "ConnectorEvent.h"
#include "../request/Request.h"
#include "../response/API/Response.h"

class ConnectorListener
{
private:

protected:

public:
	ConnectorListener();
	virtual ~ConnectorListener();
	virtual void onIncomming(ConnectorEvent e)=0;
	virtual void onDataReceiving(ConnectorEvent e)=0;
//	virtual bool operator==(const ConnectorListener &other)=0;

};

