#pragma once

#include "../request/Request.h"
#include "../response/Response.h"
#include "ConnectorEvent.h"

class ConnectorListener
{
private:
//	ConnectorListener(const ConnectorListener &other);
//	ConnectorListener& operator=(const ConnectorListener &other);

protected:
//	ConnectorListener();
//	virtual ~ConnectorListener();
public:
	virtual void onIncomming(ConnectorEvent e)=0;
	virtual void onDataReceiving(ConnectorEvent e)=0;
//	virtual bool operator==(const ConnectorListener &other)=0;

};

