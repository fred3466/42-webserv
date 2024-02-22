#pragma once

class Request
{
private:

public:
	Request();
	~Request();
//	Request(Request const &o);
//	Request& operator=(Request const &o);

	std::string getValue(std::string paramName);
	void addParam(std::string paramName, std::string paramValue);
};

