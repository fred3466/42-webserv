#pragma once
#include <string>
#include "../Processor.h"

class LocationToProcessor
{
private:
	std::string extension; // cle
	Processor *processor;  // value

public:
	LocationToProcessor();
	LocationToProcessor(std::string ext, Processor *proc);
	~LocationToProcessor();
	std::string const &getExtension() const;
	void setExtension(std::string const &extension);
	Processor const *&getProcessor() const;
	void setProcessor(Processor const *&processor);
};
