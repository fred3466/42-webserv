#include "ProcessorImplDirectFs.h"

#include <iostream>
#include <vector>

#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstring>

std::string ProcessorImplDirectFs::dir_list_preContent = "";
std::string ProcessorImplDirectFs::dir_list_postContent = "";
std::string templateLine = "";

ProcessorImplDirectFs::ProcessorImplDirectFs(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper() {
	this->type = type;
}
ProcessorImplDirectFs::~ProcessorImplDirectFs() {
	delete config;
}

void ProcessorImplDirectFs::setConfig(Config *conf) {
	config = conf;
}

Response* ProcessorImplDirectFs::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) nextProcessorAndLocationToProcessor;
	FileUtil fu = FileUtil();

	std::string basePath = config->getParamStr("base_path", "base_path");

	Uri uri = request->getUri();
	bool bProcIsRedirect = isRedirect();
	bool bIsDirValReq = isUriDirectoryValidationRequired();
	bool bIsDir = uri.isDirectory();

	if (bIsDirValReq && (!bProcIsRedirect) && bIsDir) {
		harl.debug("ProcessorImplDirectFs::process requested URI is a dir for [%s]", uri.getUri().c_str());
		std::string defaultIndexName = getProperty("default_index_name", "NONE");
		harl.debug("ProcessorImplDirectFs::process defaultIndexName=[%s]", defaultIndexName.c_str());
		if (defaultIndexName == "NONE") {
			harl.warning("ProcessorImplDirectFs::process : defaultIndexName=NONE => 404", uri.getUri().c_str());
			response->setErrorCodeTmp(404);
			return response;
		} else if (defaultIndexName != "LIST") {
			uri.setFileNameAndExt(defaultIndexName);
			request->getHeader()->setUri(uri);
			harl.debug("ProcessorImplDirectFs::process UPDATED uri=[%s]", uri.getUri().c_str());
		}
	}

	LocationToProcessor *locationToProcessor = processorAndLocationToProcessor->getLocationToProcessor();
	std::string patPath = locationToProcessor->getUrlPath();

	std::string
	uriLessPathInfoAndQueryString = request->getUri().getPathFileNameAndExt();
	processorHelper.suppressRouteFromURI(processorAndLocationToProcessor, &uriLessPathInfoAndQueryString);

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string path = fu.realPathFile(base_path + "/" + uriLessPathInfoAndQueryString);

	harl.info("ProcessorImplDirectFs::process: " + uri.getUri() + " -> " + path.c_str());

	struct stat _stat;
	if (stat(path.c_str(), &_stat) == 0) {

//		DIRECTORY
		if (_stat.st_mode & S_IFDIR) {
			response->getHeader()->addField("Content-Type", "text/html;");

			if (dir_list_preContent == "") {
				dir_list_preContent = fu.readTextFile("conf/dir_list_pre.html");
				dir_list_preContent = stringUtil.replace_all(dir_list_preContent, "[URL_BASE]", uri.getPath());

				dir_list_postContent = fu.readTextFile("conf/dir_list_post.html");
				templateLine = fu.readTextFile("conf/dir_list_line_template.html");

				if ((dir_list_preContent == "") || (dir_list_postContent == "")) {
				}
			}

			std::string bodyStr = dir_list_preContent;

			std::string sending = "<ul>";
			bodyStr += sending;
			std::string dir = uri.getPath();

			std::vector<std::string> files = fu.listDir(path);
			for (unsigned int i = 0; i < files.size(); i++) {
				sending = templateLine;
				sending = stringUtil.replace_all(sending, "[HREF_FULL]", dir + "/" + files[i]);
				sending = stringUtil.replace_all(sending, "[HREF]", files[i]);
				sending = stringUtil.replace_all(sending, "[LIB]", dir + "/" + files[i]);

				bodyStr += sending;
			}
			sending = "</ul>";
			bodyStr += sending;

			bodyStr += dir_list_postContent;

			int bodyStrSize = bodyStr.size();
			char *bodyBin = new char[bodyStrSize];
			response->setBodyLength(bodyStrSize);
			bodyStr.copy(bodyBin, bodyStrSize, 0);
			response->setBodyBin(bodyBin);
//		FILE
		}
		else if (_stat.st_mode & S_IFREG) {
			std::string fileExt = path.substr(path.rfind(".", std::string::npos));

			char *bodyBin;
			int len = fu.readFile(path, &bodyBin);
			response->setBodyLength(len);
			response->setBodyBin(bodyBin);

			if (!response->isCgi() && fu.fileExists(path)) {
				std::string dateLastModification = fu.getLastModification(path, RFC_1123_DATE_FORMAT);
				response->getHeader()->addField("Last-Modified", dateLastModification);
//				response->getHeader()->addField("ETag", ""); // 65f5bc3b-17e9

			} else {
				// something else
			}
		}
	}
	else {
		// error
		harl.warning("ProcessorImplDirectFs::process : %s n'existe pas.", request->getUri().getUri().c_str());
		response->setErrorCodeTmp(404);
	}
	return response;
}

bool ProcessorImplDirectFs::isRedirect() {
	return false;
}

void ProcessorImplDirectFs::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplDirectFs::toString() {
	return "ProcessorImplDirectFs";
}

ProcessorTypeEnum ProcessorImplDirectFs::getType() {
	return type;
}

bool ProcessorImplDirectFs::isExclusif() {
	return false;
}

bool ProcessorImplDirectFs::isBypassingExclusif() {
	return false;
}

bool ProcessorImplDirectFs::isUriDirectoryValidationRequired() {
	return true;
}

bool ProcessorImplDirectFs::isCgi() {
	return false;
}

std::string ProcessorImplDirectFs::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* ProcessorImplDirectFs::getConfig() {
	return config;
}
