#pragma once

#include "../connector/RequestResponseParam.h"
#include "Response.h"

class ResponseProducer {
private:

protected:

public:
	ResponseProducer();
	virtual ~ResponseProducer();
	virtual Response* createResponse(RequestResponseParam *e)=0;

};

