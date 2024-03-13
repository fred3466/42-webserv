#include "LocationToProcessor.h"

LocationToProcessor::LocationToProcessor() : extension(""), processor(NULL)
{
}

LocationToProcessor::LocationToProcessor(std::string ext, Processor *proc)
{
	extension = ext;
	processor = proc;
}

LocationToProcessor::~LocationToProcessor()
{
}

std::string const &LocationToProcessor::getExtension() const
{
	return extension;
}

void LocationToProcessor::setExtension(std::string const &ext)
{
	extension = ext;
}

Processor const *&LocationToProcessor::getProcessor() const
{
	return processor;
}

void LocationToProcessor::setProcessor(Processor const *&proc)
{
	processor = proc;
}

std::string const &LocationToProcessor::getExtension() const
{
	return extension;
}

void LocationToProcessor::setExtension(std::string const &extension)
{
	this->extension = extension;
}

Processor const *&LocationToProcessor::getProcessor() const
{
	return processor;
}

void LocationToProcessor::setProcessor(Processor const *&processor)
{
	this->processor = processor;
}

LocationToProcessor::~LocationToProcessor()
{
}
