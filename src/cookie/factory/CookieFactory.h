#pragma once

#include "../Cookie.h"
#include "../../API/RequestHeader.h"

class CookieFactory {
private:
public:
	CookieFactory();
	~CookieFactory();
	void build(RequestHeader *header);
};
