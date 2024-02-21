#pragma once
#include "ConnectorListener.h"

class ConnectorEventPublisher {
private:
	ConnectorEventPublisher();
	~ConnectorEventPublisher();
	ConnectorEventPublisher(const ConnectorEventPublisher &other);
	ConnectorEventPublisher& operator=(const ConnectorEventPublisher &other);
public:
	virtual void registerIt(ConnectorListener l)=0;
	virtual void unregisterIt(ConnectorListener l)=0;
	virtual void publishAccepting(ConnectorEvent e)=0;
	virtual void unregisterIt(ConnectorEvent e)=0;
protected:
};

