#include "LocationToProcessor.h"

LocationToProcessor::LocationToProcessor() : urlPath(""), extension(""), processor(NULL), host("")
{
}

LocationToProcessor::LocationToProcessor(std::string urlPath, std::string ext, Processor *proc, std::string host)
{
	extension = ext;
	processor = proc;
	this->urlPath = urlPath;
	this->host = host;
//	this->port = port;
}

LocationToProcessor::~LocationToProcessor()
{

}

LocationToProcessor::LocationToProcessor(LocationToProcessor &bis) : extension(bis.extension), processor(bis.processor), urlPath(
		bis.urlPath), host(bis.host)
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
		host = bis.host;
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
	return ("LocationToProcessor : [" + host + "] [" + urlPath + "] \t [" + extension + "] \t [" + getProcessor()->toString() + "]").c_str();
}

std::string LocationToProcessor::getHost()
{
	return host;
}

void LocationToProcessor::setHost(std::string &host)
{
	this->host = host;
}
