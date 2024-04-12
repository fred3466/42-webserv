#pragma once
#include <string>
#include "../../config.h"

#include "../../Harl.h"
#include "CGIHandler.h"

#include CGI_HANDLER_INC_PHP
//#include CGI_HANDLER_INC_SH
#include CGI_HANDLER_INC_PERL

class CGIHandlerFactory
{
private:
	Harl harl;
	public:
	~CGIHandlerFactory();
	CGIHandlerFactory();
	CGIHandler* build(std::string cgiName, Config *config);
};
