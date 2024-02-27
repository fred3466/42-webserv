#pragma once
#include "Processor.h"
#include "ProcessorImplDirectFs.h"
#include "../response/ResponseFactory.h"
#include "../config/Config.h"
#include "../util/FileUtilFactory.h"
#include "../Harl.h"

class ProcessorImplDirectFs: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
public:
	ProcessorImplDirectFs();
	~ProcessorImplDirectFs();
	Response* process(Request *request);
	void setConfig(Config *conf);

};

