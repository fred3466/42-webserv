#include <iostream>
#include <vector>

#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>

#include "ProcessorImplDirectFs.h"

ProcessorImplDirectFs::ProcessorImplDirectFs() :
		harl()
{
}
ProcessorImplDirectFs::~ProcessorImplDirectFs()
{
}

void ProcessorImplDirectFs::setConfig(Config *conf)
{
	config = conf;
}

Response* ProcessorImplDirectFs::process(Request *request)
{
	Response *resp = ResponseFactory().build();
	FileUtil *fu = FileUtilFactory().build();

//	std::string path = "C:\\Users\\Sauleyayan\\Desktop\\New folder";

	std::string root = config->getParamStr("root");
	std::string path = root + request->getUri();
	harl.info(request->getUri() + " -> " + path);

	std::string body = "";

	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			std::vector<std::string> files = fu->listDir(path);
			for (unsigned int i = 0; i < files.size(); i++)
			{
				std::string sending = "<a href=\"" + files[i] + "\">" + files[i]
						+ "</a>\n";

				body += sending;
			}
//			bodyStr.copy(body, bodyStr.size(), 0);
			resp->setBodyLength(body.size());

		} else if (s.st_mode & S_IFREG)
		{
			int len = fu->readFile(path, body);
			resp->setBodyLength(len);
//			send(request->getFdClient(), ret.c_str(), ret.size(), 0);

//			fin >> rno >> name >> fee;   //read data from the file student

//			is.close();

		} else
		{
			// something else
		}
	} else
	{
		// error
	}
//    _response_content.append("HTTP/1.1 " + toString(_code) + " ");
//    _response_content.append(statusCodeString(_code));
//    _response_content.append("\r\n");
	resp->setStatusLine("HTTP/1.1 200 OK\r\n");
//	resp->setBody("<html><body>" + body + "</body></html>");
	resp->setBody(body);
	return resp;
}
