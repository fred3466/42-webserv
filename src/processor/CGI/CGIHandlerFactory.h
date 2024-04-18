#pragma once
#include <string>

#include "../../API/CGIHandler.h"
#include "../../config.h"

#include "../../Harl.h"
#include "impl/CGIHandlerPerl.h"
#include "impl/CGIHandlerPHP.h"
#include "impl/CGIHandlerSh.h"

class CGIHandlerFactory
{
private:
	Harl harl;
	public:
	~CGIHandlerFactory();
	CGIHandlerFactory();
	CGIHandler* build(std::string cgiName, Config *config);
};
