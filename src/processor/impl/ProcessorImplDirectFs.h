#pragma once
#include "../../API/ResponseHeader.h"
#include "../../Harl.h"
#include "../../response/factory/ResponseHeaderFactory.h"

#include "../../util/FileUtilFactory.h"
#include "../../util/StringUtil.h"
#include "../../config/Config.h"
#include "../../response/factory/ResponseFactory.h"
#include "../../location/ProcessorAndLocationToProcessor.h"
#include "../../API/Processor.h"
#include "../ProcessorHelper.h"

class ProcessorImplDirectFs: public Processor {
private:
	static std::string dir_list_preContent;
	static std::string dir_list_postContent;
	Harl harl;
	StringUtil stringUtil;
	Config *config;
	FileUtil fileUtil;
	ProcessorTypeEnum type;
	ProcessorHelper processorHelper;

public:
	ProcessorImplDirectFs(ProcessorTypeEnum type);
	~ProcessorImplDirectFs();
	Response* process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor, ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual ProcessorTypeEnum getType();
	virtual bool isExclusif();
	virtual bool isBypassingExclusif();
	virtual std::string getProperty(std::string name, std::string defaultVal);
	virtual Config* getConfig();
	virtual bool isRedirect();
	virtual bool isCgi();
	virtual bool isUriDirectoryValidationRequired();
};
