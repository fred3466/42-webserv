#pragma once

#include <map>
#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "Processor.h"
#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/factory/ResponseFactory.h"
<<<<<<< HEAD
=======
#include "CGI/CGIHandler.h"
// #include "../location/LocationToProcessor.h"
>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git
#include "../location/ProcessorAndLocationToProcessor.h"
<<<<<<< HEAD
#include "CGI/CGIHandlerPHP.h"
=======
#include "../error/HttpErrorFactory.h"
// #include "../error/HttpError.h"
// #include "../error/HttpReturnCodeHelper.h"
>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git

// class ProcessorAndLocationToProcessor;
class LocationToProcessor;

class ProcessorImplCgiSh : public Processor
{
private:
	Harl harl;
	StringUtil stringUtil;
	Config *config;
	FileUtil fileUtil;
	ProcessorTypeEnum type;
	//	std::map<std::string, std::string> env;

protected:
public:
	ProcessorImplCgiSh(ProcessorTypeEnum type);
	~ProcessorImplCgiSh();
	virtual Response *process(Request *request, Response *response,
							  ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual ProcessorTypeEnum getType();
	void addProperty(std::string name, std::string value);

	std::string readRequest(int clientFd);
	//	void closeClient(int clientFd);
	//	int getListenFd();
	//	bool isCGIRequest(const std::string &uri);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
	//	int getClientFd(int clientId);
	std::string getBasePath();
	void setBasePath(std::string basePath);
};
