#pragma once
#include <string>

#include "../../API/CGIHandler.h"
#include "../../config.h"

class CGIHandlerFactory {
private:
	Harl harl;
	public:
	~CGIHandlerFactory();
	CGIHandlerFactory();
	CGIHandler* build(std::string cgiName, Config *config);
};
