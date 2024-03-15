#include "LocationToProcessor.h"

LocationToProcessor::LocationToProcessor() : urlPath(""), extension(""), processor(NULL)
{
}

LocationToProcessor::LocationToProcessor(std::string urlPath, std::string ext, Processor *proc)
{
	extension = ext;
	processor = proc;
	this->urlPath = urlPath;
}

LocationToProcessor::~LocationToProcessor()
{

}

LocationToProcessor::LocationToProcessor(LocationToProcessor &bis) : extension(bis.extension), processor(bis.processor), urlPath(
		bis.urlPath)
{
	*this = bis;
}
LocationToProcessor& LocationToProcessor::operator=(LocationToProcessor &bis)
{
	if (this != &bis)
	{
		extension = bis.extension;
		processor = bis.processor;
		urlPath = bis.urlPath;
	}
	return *this;
}

std::string LocationToProcessor::getExtension()
{
	return extension;
}

void LocationToProcessor::setExtension(std::string ext)
{
	extension = ext;
}

Processor* LocationToProcessor::getProcessor()
{
	return processor;
}

void LocationToProcessor::setProcessor(Processor *proc)
{
	processor = proc;
}

std::string LocationToProcessor::getUrlPath()
{
	return urlPath;
}

void LocationToProcessor::setUrlPath(std::string urlPath)
{
	this->urlPath = urlPath;
}

std::string LocationToProcessor::toString()
{
	return ("LocationToProcessor : [" + urlPath + "] \t [" + extension + "] \t [" + getProcessor()->toString() + "]").c_str();
}
