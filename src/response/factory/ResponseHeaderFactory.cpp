#include "ResponseHeaderFactory.h"
#include "../../config.h"
#include "../../Harl.h"

ResponseHeaderFactory::ResponseHeaderFactory() {
}

ResponseHeaderFactory::~ResponseHeaderFactory() {
}
ResponseHeader* ResponseHeaderFactory::build() {
	Harl().debug("ResponseHeaderFactory::build +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");

	return new RESPONSE_HEADER_IMPL_CLASS();
}
