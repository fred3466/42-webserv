#include "ProcessorImplCgiBinPerl.h"

ProcessorImplCgiBinPerl::ProcessorImplCgiBinPerl(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper()
{
	this->type = type;
}
ProcessorImplCgiBinPerl::~ProcessorImplCgiBinPerl()
{
	harl.debug("ProcessorImplCgiBinPerl::~ProcessorImplCgiBinPerl: destruction de  Config dans ProcessorImplCgiBinPerl");
	delete config;
}

void ProcessorImplCgiBinPerl::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplCgiBinPerl::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	//	TODO fred post
	response->getHeader()->addField("Content-Type", "text/html;");

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	CGIHandler *cgiHandler = CGIHandlerFactory().build("PERL_CGI", config);
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

std::string ProcessorImplCgiBinPerl::getBasePath()
{
	return config->getParamStr("base_path", "cgi-bin");
}

void ProcessorImplCgiBinPerl::setBasePath(std::string basePath)
{
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinPerl::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplCgiBinPerl::toString()
{
	return "ProcessorImplCgiBinPerl";
}

ProcessorTypeEnum ProcessorImplCgiBinPerl::getType()
{
	return type;
}

bool ProcessorImplCgiBinPerl::isExclusif()
{
	return true;
}

bool ProcessorImplCgiBinPerl::isBypassingExclusif()
{
	return false;
}

