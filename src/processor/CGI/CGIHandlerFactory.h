#pragma once
#include <string>

#include "../../API/CGIHandler.h"
#include "../../config.h"

#include "../../Harl.h"
#include CGI_HANDLER_INC_PERL
//#include "impl/CGIHandlerPHP.h"
#include CGI_HANDLER_INC_SH
#include "impl/CGIHandlerGeneric.h"

class CGIHandlerFactory
{
private:
	Harl harl;
	public:
	~CGIHandlerFactory();
	CGIHandlerFactory();
	CGIHandler* build(std::string cgiName, Config *config);
};
