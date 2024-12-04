#pragma once
#include <vector>
#include <algorithm>

#include "LocationToProcessor.h"
#include "ProcessorAndLocationToProcessor.h"
#include "../Harl.h"
#include "../util/StringUtil.h"

class ProcessorLocator {
private:
	Harl harl;
	static std::vector<LocationToProcessor*> locationToProcessorVector;

public:
	ProcessorLocator();
	~ProcessorLocator();
	bool operator<(ProcessorLocator &o);
	std::vector<LocationToProcessor*> getLocationToProcessorVector();
	void addLocationToProcessor(std::string urlPath, std::string ext, Processor *processor, std::string host, std::string hostAsIpPort, std::string method);
	std::vector<ProcessorAndLocationToProcessor*>* listOrderedProcessorForUrlAndExt(Request *request);
	bool _checkAccess(Request *request, Response *resp, Processor *processor);
};

