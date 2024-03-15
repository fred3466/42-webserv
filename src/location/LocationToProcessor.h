#pragma once
#include <string>

#include "../processor/Processor.h"

class LocationToProcessor
{
private:
	std::string urlPath;
	std::string extension;
	Processor *processor;

public:
	LocationToProcessor();
	LocationToProcessor(std::string urlPath, std::string ext, Processor *proc);
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
};

