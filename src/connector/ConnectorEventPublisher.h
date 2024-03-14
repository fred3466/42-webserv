#pragma once
#include "ConnectorListener.h"

class ConnectorEventPublisher
{
private:
	ConnectorEventPublisher();
	~ConnectorEventPublisher();

public:
	virtual void registerIt(ConnectorListener l)=0; // @suppress("Abstract class cannot be instantiated")
	virtual void unregisterIt(ConnectorListener l)=0; // @suppress("Abstract class cannot be instantiated")
	virtual void publishAccepting(ConnectorEvent e)=0;
	virtual void unregisterIt(ConnectorEvent e)=0;

protected:
};

