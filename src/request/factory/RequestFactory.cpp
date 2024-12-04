#include "RequestFactory.h"

RequestFactory::RequestFactory() {
}

RequestFactory::~RequestFactory() {
}

Request* RequestFactory::build(RequestHeader *header, RequestBody *body) {
	Request *req = new REQUEST_IMPL_CLASS(header, body);

	return req;
}
