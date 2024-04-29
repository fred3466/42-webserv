#include "ProcessorImplDirectFs.h"

#include <iostream>
#include <vector>

#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstring>

ProcessorImplDirectFs::ProcessorImplDirectFs(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper()
{
	this->type = type;
}
ProcessorImplDirectFs::~ProcessorImplDirectFs()
{
}

void ProcessorImplDirectFs::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplDirectFs::process(Request *request, Response *response,
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	FileUtil fu = FileUtil();

	std::string basePath = config->getParamStr("base_path", "base_path");

	//	TODO factoriser
	Uri uri = request->getUri();
	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();

	std::string
	uriLessPathInfoAndQueryString = request->getUri().getPath() + request->getUri().getFileName() + request->getUri().getFileExtension();
	processorHelper.suppressRouteFromURI(processorAndLocationToProcessor, &uriLessPathInfoAndQueryString);

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string path = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uriLessPathInfoAndQueryString;
	harl.debug(toString() + ":\t" + request->getUri().getUri() + " -> " + path);
	//	TODO FIN factoriser

	harl.info("ProcessorImplDirectFs::process: " + request->getUri().getUri() + " -> " + path);

	struct stat _stat;
	if (stat(path.c_str(), &_stat) == 0)
	{
		std::string bodyStr = "";

//		DIRECTORY
		if (_stat.st_mode & S_IFDIR)
		{
			std::vector<std::string> files = fu.listDir(path);
			for (unsigned int i = 0; i < files.size(); i++)
			{
				std::string sending = "<a href=\"" + files[i] + "\">" + files[i] + "</a>\n";

				bodyStr += sending;
			}
			int bodyStrSize = bodyStr.size();
			char *body = new char[bodyStrSize];
			response->setBodyLength(bodyStrSize);
			bodyStr.copy(body, bodyStrSize, 0);
//		FILE
		} else if (_stat.st_mode & S_IFREG)
		{
			std::string fileExt = path.substr(
					path.rfind(".", std::string::npos));

			char *bodyBin;
			int len = fu.readFile(path, &bodyBin);
			response->setBodyLength(len);
			response->setBodyBin(bodyBin);

			if (!response->isCgi() && fu.fileExists(path))
			{
				std::string dateLastModification = fu.getLastModification(path, RFC_1123_DATE_FORMAT);
				response->getHeader()->addField("Last-Modified", dateLastModification);
//				response->getHeader()->addField("ETag", ""); // 65f5bc3b-17e9

			}
			else
			{
				// something else
			}
		}
	} else
	{
		// error
		harl.warning("ProcessorImplDirectFs::process : %s n'existe pas.", path.c_str());
		// response->getHeader()->setStatusLine("HTTP/1.1 404 NOT FOUND\r\n");
		response->setErrorCodeTmp(404);
	}
	//---------------------testing cooking------------------
	/*	Cookie cookie;
	 cookie.setName("TEST");
	 cookie.setValue("42");
	 long age = 3600;
	 cookie.setMaxAge(age);
	 cookie.setDomain("localhost");
	 cookie.setPath("/");
	 resp->getHeader()->addCookie(cookie);

	 cookie.setName("SID");
	 cookie.setValue("123456");
	 age = 3600;
	 cookie.setMaxAge(age);
	 cookie.setDomain("localhost");
	 cookie.setPath("/");
	 resp->getHeader()->addCookie(cookie);

	 cookie.setName("VAL");
	 cookie.setValue("bonjour");
	 age = 3600;
	 cookie.setMaxAge(age);
	 cookie.setDomain("localhost");
	 cookie.setPath("/");
	 resp->getHeader()->addCookie(cookie);*/
	//-----------------------------------------------------
	//    _response_content.append("HTTP/1.1 " + toString(_code) + " ");
	//    _response_content.append(statusCodeString(_code));
	//    _response_content.append("\r\n");
	////	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
	//	resp->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
	////	resp->setBody("<html><body>" + body + "</body></html>");
	//	resp->setBody(body);
	return response;
}

void ProcessorImplDirectFs::addProperty(std::string name, std::string value)
{
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplDirectFs::toString()
{
	return "ProcessorImplDirectFs";
}

ProcessorTypeEnum ProcessorImplDirectFs::getType()
{
	return type;
}

bool ProcessorImplDirectFs::isExclusif()
{
	return false;
}

bool ProcessorImplDirectFs::isBypassingExclusif()
{
	return false;
}

std::string ProcessorImplDirectFs::getProperty(std::string name, std::string defaultVal)
{
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* ProcessorImplDirectFs::getConfig()
{
	return config;
}
