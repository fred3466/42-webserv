#pragma once
#include "../ResponseHttpHeader.h"
class ResponseHeaderFactory
{
public:
	ResponseHeaderFactory();
	~ResponseHeaderFactory();
	ResponseHeader* build();
};

