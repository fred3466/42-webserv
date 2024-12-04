#include "CGIHandlerFactory.h"
#include CGI_HANDLER_INC_PERL
#include CGI_HANDLER_INC_SH
#include CGI_HANDLER_INC_GENERIC

CGIHandler* CGIHandlerFactory::build(std::string cgiName, Config *config) {
	CGIHandler *cgi = NULL;
	if (cgiName == "PERL_CGI")
		cgi = new CGI_HANDLER_CLASS_PERL();
	else if (cgiName == "SH_CGI")
		cgi = new CGI_HANDLER_CLASS_SH();
	else if (cgiName == "GENERIC_EXE") {
		cgi = new CGI_HANDLER_CLASS_GENERIC();
	}
	cgi->setConfig(config);
	harl.debug("CGIHandlerFactory::build: return %s", cgi->toString().c_str());
	return cgi;
}

CGIHandlerFactory::CGIHandlerFactory() : harl() {
}

CGIHandlerFactory::~CGIHandlerFactory() {
}

