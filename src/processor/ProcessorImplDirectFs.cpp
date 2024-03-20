#include "ProcessorImplDirectFs.h"

#include <iostream>
#include <vector>

#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstring>

ProcessorImplDirectFs::ProcessorImplDirectFs(ProcessorTypeEnum type) : harl(), stringUtil(), config()
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

Response *ProcessorImplDirectFs::process(Request *request, Response *response,
										 ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);
	FileUtil *fu = FileUtilFactory().build();

	//	std::string path = "C:\\Users\\Sauleyayan\\Desktop\\New folder";

	std::string basePath = config->getParamStr("base_path", "base_path");
	//	std::string path = root + request->getUri();

	//	TODO factoriser
	std::string uri = request->getUri();
	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();
	int patPathLen = patPath.length();
	size_t ite = uri.find(patPath);
	if (ite == 0)
	{
		uri.erase(0, patPathLen);
	}

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string path = config->getParamStr("ROOT_PATH", "./") + "/" + base_path + uri;
	harl.debug(toString() + ":\t" + request->getUri() + " -> " + path);
	//	TODO FIN factoriser

	harl.info(request->getUri() + " -> " + path);
	char *body;

	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		std::string bodyStr = "";
		if (s.st_mode & S_IFDIR)
		{
			std::vector<std::string> files = fu->listDir(path);
			for (unsigned int i = 0; i < files.size(); i++)
			{
				std::string sending = "<a href=\"" + files[i] + "\">" + files[i] + "</a>\n";

				bodyStr += sending;
			}
			resp->setBodyLength(bodyStr.size());
			bodyStr.copy(body, bodyStr.size(), 0);
			//			body = bodyStr.c_str();
		}
		else if (s.st_mode & S_IFREG)
		{
			std::string fileExt = path.substr(
				path.rfind(".", std::string::npos));

			if (stringUtil.strUpperCase(fileExt) == ".GIF")
			{
				resp->getHeader()->addField("Content-Type", "image/gif");
			}
			else if (stringUtil.strUpperCase(fileExt) == ".HTML")
			{
				resp->getHeader()->addField("Content-Type", "text/html");
			}
			else if (stringUtil.strUpperCase(fileExt) == ".PHP")
			{
				resp->getHeader()->addField("Content-Type", "text/html");
			}
			//			else if (stringUtil.strUpper(fileExt) == ".PNG")
			//			{
			//				resp->getHeader()->addField("Content-Type: image/png\r\n");
			//			}
			else if (stringUtil.strUpperCase(fileExt) == ".JPEG" || stringUtil.strUpperCase(fileExt) == ".JPG")
			{
				resp->getHeader()->addField("Content-Type", "image/jpeg");
			}

			char *bodyBin;
			int len = fu->readFile(path, &bodyBin);
			resp->setBodyLength(len);
			resp->setBodyBin(bodyBin);

			// std::ofstream out("out2.gif", std::ios::out | std::ios::binary);
			// out.write(bodyBin, len);
			// out.close();

			body = bodyBin;
			//			std::ofstream os("out2.gif", std::ios::binary | std::ios::out);
			//			os.write(body, len);
			//			os.close();
			//			bodyStr.copy(body, bodyStr.size(), 0);

			//			send(request->getFdClient(), ret.c_str(), ret.size(), 0);

			//			fin >> rno >> name >> fee;   //read data from the file student

			//			is.close();
		}
		else
		{
			// something else
		}
		//	TODO : adapter le code retour HTTP dans la réponse, au résultat de l'exécution de process()
		resp->getHeader()->setStatusLine("HTTP/1.1 200 OK\r\n");
	}
	else
	{
		// error
		harl.warning("ProcessorImplDirectFs::process : %s n'existe pas.", path.c_str());
		resp->getHeader()->setStatusLine("HTTP/1.1 404 NOT FOUND\r\n");
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
	delete fu;
	return resp;
}

void ProcessorImplDirectFs::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

std::string ProcessorImplDirectFs::toString()
{
	return "ProcessorImplDirectFs";
}

ProcessorTypeEnum ProcessorImplDirectFs::getType()
{
	return type;
}
