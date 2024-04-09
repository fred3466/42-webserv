#include "CGIHandlerFactory.h"

CGIHandler* CGIHandlerFactory::build(std::string cgiName, Config *config)
{
	CGIHandler *cgi = NULL;
	if (cgiName == "PHP_CGI")
	{
		cgi = new CGI_HANDLER_CLASS_PHP();
	}
	else if (cgiName == "PERL_CGI")
		cgi = new CGI_HANDLER_CLASS_PERL();
	else if (cgiName == "SH_CGI")
		cgi = new CGI_HANDLER_CLASS_SH();
//	else if (cgiName == "PHP_LEG_CGI")
//		return new REQUEST_HANDLER_IMPL_CLASS_STATIC();
	cgi->setConfig(config);
	harl.debug("CGIHandlerFactory::build: return %s", cgi->toString().c_str());
	return cgi;
}

CGIHandlerFactory::CGIHandlerFactory() : harl()
{
}

CGIHandlerFactory::~CGIHandlerFactory()
{
}
