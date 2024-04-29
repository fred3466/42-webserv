#include "FiltreRequestPost.h"

FiltreRequestPost::FiltreRequestPost(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper()
{
	this->type = type;
}

FiltreRequestPost::~FiltreRequestPost()
{
}

void FiltreRequestPost::setConfig(Config *conf)
{
	config = conf;
}

ProcessorTypeEnum FiltreRequestPost::getType()
{
	return type;
}

std::string FiltreRequestPost::toString()
{
	return "FiltreRequestPost";
}
// TODO http error
/*
 If CONTENT_TYPE is blank, the script can reject the request
 with a 415 'Unsupported Media Type' error, where supported by the
 protocol.

 When parsing PATH_INFO, PATH_TRANSLATED or SCRIPT_NAME the script
 should be careful of void path segments ("//") and special path
 segments ("." and "..").  They should either be removed from the path
 before use in OS system calls, or the request should be rejected with
 404 'Not Found'.
 */
Response* FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void) processorAndLocationToProcessor;
	RequestBody *body = request->getBody();
	//	response->set
	std::string CONTENT_TYPE = request->getHeaderFieldValue("Content-Type");
	std::string multiStr = "multipart/form-data";
	int pos = 0;
	int len = multiStr.length();
	int multiStrMatch = (0 == CONTENT_TYPE.compare(pos, len, multiStr));

	if (CONTENT_TYPE == "application/x-www-form-urlencoded" || multiStrMatch)
	{
		//		POST POST POST POST POST POST POST POST POST POST POST
		std::string basePath = config->getParamStr("base_path", "base_path");
		basePath = fileUtil.realPathFile(basePath);
		std::vector<std::string> vecTemp = stringUtil.tokenize(CONTENT_TYPE, ';');
		std::string boundary = vecTemp.at(1);
		boundary = stringUtil.replace_all(boundary, " boundary=", "");
		MultipartParser parser = MultipartParser(boundary);
//		const std::string &body=
		parser.parseMultipartData(*(body)->getContent(), basePath);

//		std::string path = basePath + "/" + "test.jpeg";
//		RequestBody *body = request->getBody();
//		char *data = (char*) body->getContent()->c_str();
//		int len = body->getContent()->length();
//		fileUtil.writeFile(path, data, len);

		std::string CONTENT_LENGTH = request->getHeaderFieldValue("Content-Length");
	}

	return response;
}

void FiltreRequestPost::addProperty(std::string name, std::string value)
{
	config->addOrReplaceParam(name, value);
}

bool FiltreRequestPost::isExclusif()
{
	return false;
}

bool FiltreRequestPost::isBypassingExclusif()
{
	return false;
}

std::string FiltreRequestPost::getProperty(std::string name, std::string defaultVal)
{
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreRequestPost::getConfig()
{
	return config;
}
