#include "ProcessorImplCgiBinSh.h"

ProcessorImplCgiBinSh::ProcessorImplCgiBinSh(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper()
{
	this->type = type;
}
ProcessorImplCgiBinSh::~ProcessorImplCgiBinSh()
{
}

void ProcessorImplCgiBinSh::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiBinSh::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	response->getHeader()->addField("Content-Type", "text/html;");

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	CGIHandler *cgiHandler = CGIHandlerFactory().build("SH_CGI", config);
	std::string
	uriLessPathInfoAndQueryString = request->getUri().getPath() + request->getUri().getFileName() + request->getUri().getFileExtension();

	processorHelper.suppressRouteFromURI(processorAndLocationToProcessor, &uriLessPathInfoAndQueryString);
	std::string interpreterPath, scriptPath;
	processorHelper.setInterpreterAndScriptPath(&interpreterPath, &scriptPath, request, config, harl, (Processor*) this, base_path, uriLessPathInfoAndQueryString);

	std::string
	cgiOutput = cgiHandler->executeCGIScript(interpreterPath, scriptPath, request, response);

	delete cgiHandler;

	//	TODO fred ???
	bool bTransferEncoding = true; // "" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
	processorHelper.applyTransferEncodingOrContentLengthAndFinalize(response, &cgiOutput, bTransferEncoding);
	return response;
}

std::string ProcessorImplCgiBinSh::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinSh::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinSh::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinSh::toString()
{
	return "ProcessorImplCgiBinSh";
}

ProcessorTypeEnum ProcessorImplCgiBinSh::getType()
{
	return type;
}

bool ProcessorImplCgiBinSh::isExclusif()
{
	return true;
}

bool ProcessorImplCgiBinSh::isBypassingExclusif()
{
	return false;
}

std::string ProcessorImplCgiBinSh::getProperty(std::string name, std::string defaultVal)
{
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}
