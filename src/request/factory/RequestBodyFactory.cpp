#include "RequestBodyFactory.h"

RequestBodyFactory::RequestBodyFactory() {
}

RequestBodyFactory::~RequestBodyFactory() {
}
RequestBody* RequestBodyFactory::build(char *rawRequest, int rawRequestTotalLen, int bodyLen, RequestHeader *reqHeader) {
	std::string methodName = reqHeader->getMethod();

	if (methodName == "POST") {

		return new REQUEST_BODY_IMPL_CLASS(rawRequest, rawRequestTotalLen, bodyLen);
	}
	return new REQUEST_BODY_IMPL_CLASS(NULL, 0, rawRequestTotalLen);
}
