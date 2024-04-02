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
	if (CONTENT_TYPE == "application/x-www-form-urlencoded"
			|| CONTENT_TYPE == "multipart/form-data")
	{
		//		POST POST POST POST POST POST POST POST POST POST POST

		std::string CONTENT_LENGTH = reqHeader->getFieldValue("Content-Length");
		return new REQUEST_BODY_IMPL_CLASS(rawRequest);
	}
	return new REQUEST_BODY_IMPL_CLASS(NULL);
}
