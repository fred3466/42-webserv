#include "HttpReturnCodeHelper.h"

HttReturnCodeHelper::HttReturnCodeHelper() {}

std::string HttReturnCodeHelper::getStatusLine(int httpReturnCode)
{
    ResponseTools responseTools;
    std::string reasonPhrase = responseTools.buildStatusLineForCode(httpReturnCode);

    std::ostringstream ssHttpReturnCode;
    ssHttpReturnCode << httpReturnCode;
    std::string statusLine = "HTTP/1.1 " + ssHttpReturnCode.str() + " " + reasonPhrase + "\r\n";
    return statusLine;
}

/*
HttReturnCodeHelper helper;
std::string statusLine;

statusLine = helper.getStatusLine(200);

*/