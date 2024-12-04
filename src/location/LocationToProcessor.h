#pragma once
#include <string>

#include "../API/Processor.h"

class Processor;
class LocationToProcessor {
private:
	std::string urlPath;
	std::string extension;
	Processor *processor;
	std::string host;
	std::string hostAsIpPort;
	std::string method;

public:
	LocationToProcessor();
	LocationToProcessor(std::string urlPath, std::string ext, Processor *proc, std::string host, std::string hostAsIpPort, std::string method);
	LocationToProcessor(LocationToProcessor &bis);
	LocationToProcessor& operator=(LocationToProcessor &bis);

	~LocationToProcessor();
	std::string getExtension();
	void setExtension(std::string extension);
	Processor* getProcessor();
	void setProcessor(Processor *processor);
	std::string getUrlPath();
	void setUrlPath(std::string urlPath);
	std::string toString();
	std::string getHost();
	void setHost(std::string host);
	std::string getMethod();
	void setMethod(std::string method);
	std::string getHostAsIpPort();
	void setHostAsIpPort(std::string hostAsIpPort);
};

