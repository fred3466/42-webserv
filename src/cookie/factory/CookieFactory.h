#pragma once

#include "../Cookie.h"
#include "../../request/RequestHttpHeader.h"

class CookieFactory
{
private:
public:
	CookieFactory();
	~CookieFactory();
	void build(RequestHeader *header);
};