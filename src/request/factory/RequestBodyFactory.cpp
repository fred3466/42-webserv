#include "RequestBodyFactory.h"

RequestBodyFactory::RequestBodyFactory()
{
}

RequestBodyFactory::~RequestBodyFactory()
{
}
RequestBody* RequestBodyFactory::build(std::string *rawRequest, RequestHeader *reqHeader)
{
	std::string CONTENT_TYPE = reqHeader->getFieldValue("Content-Type");
	std::string multiStr = "multipart/form-data";
	int pos = 0;
	int len = multiStr.length();
	int multiStrMatch = (0 == CONTENT_TYPE.compare(pos, len, multiStr));

	if (CONTENT_TYPE == "application/x-www-form-urlencoded" || multiStrMatch)
	{
		//		POST POST POST POST POST POST POST POST POST POST POST

		std::string CONTENT_LENGTH = reqHeader->getFieldValue("Content-Length");
		return new REQUEST_BODY_IMPL_CLASS(rawRequest);
	}
	return new REQUEST_BODY_IMPL_CLASS(NULL);
}
