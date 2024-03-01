#include "ResponseTools.h"

ResponseTools::ResponseTools() :
		codeReasonMap()
{
	codeReasonMap[200] = "OK";
	codeReasonMap[404] = "Not Found";
	codeReasonMap[503] = "Service Unavailable";
}

ResponseTools::~ResponseTools()
{
}

std::string ResponseTools::buildStatusLineForCode(int code)
{
	return codeReasonMap[code];
}
