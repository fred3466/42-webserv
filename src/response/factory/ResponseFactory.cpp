#include "ResponseFactory.h"
#include "../../config.h"
#include "../../Harl.h"
#include "../../ServerRunner.h"

ResponseFactory::ResponseFactory() {
}

ResponseFactory::~ResponseFactory() {
}
Response* ResponseFactory::build(ResponseHeader *header) {
	Harl().debug("ResponseFactory::build +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
	Response *res = new RESPONSE_IMPL_CLASS(header);
	ServerRunner::nbResp++;
	return res;
}
