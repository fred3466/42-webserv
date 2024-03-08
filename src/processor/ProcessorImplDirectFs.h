#pragma once
#include "ProcessorImplDirectFs.h"
#include "Processor.h"

#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"

#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/factory/ResponseFactory.h"

class ProcessorImplDirectFs : public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;

public:
	ProcessorImplDirectFs();
	~ProcessorImplDirectFs();
	Response *process(Request *request);
	void setConfig(Config *conf);
};
