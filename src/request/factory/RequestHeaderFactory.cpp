#include "RequestHeaderFactory.h"
#include "../../config.h"

RequestHeaderFactory::RequestHeaderFactory() {
}

RequestHeaderFactory::~RequestHeaderFactory() {
}

RequestHeader* RequestHeaderFactory::build(char *rawRequest, int const rawRequestLen) {
	return new REQUEST_HEADER_IMPL_CLASS(rawRequest, rawRequestLen);
}
