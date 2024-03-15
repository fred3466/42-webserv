#include "Cookie.h"

Cookie::Cookie()
{
}

Cookie::~Cookie()
{
}

long Cookie::getMaxAge() const
{
	return _maxAge;
}

std::string Cookie::getPath() const
{
	return _path;
}

std::string Cookie::getDomain() const
{
	return _domain;
}

std::string Cookie::getSecure() const
{
	return _secure;
}

std::string Cookie::getHttponly() const
{
	return _httponly;
}

std::string Cookie::getName() const
{
	return _name;
}

std::string Cookie::getValue() const
{
	return _value;
}

void Cookie::setMaxAge(long &max_age)
{
	_maxAge = max_age;
}

void Cookie::setPath(const std::string &path)	
{
	_path = path;
}

void Cookie::setDomain(const std::string &domain)
{
	_domain = domain;
}

void Cookie::setSecure()
{
	_secure = "Secure";
}

void Cookie::setHttponly()
{
	_httponly = "HttpOnly";
}

void Cookie::setName(const std::string &name)
{
	_name = name;
}

void Cookie::setValue(const std::string &value)
{
	_value = value;
}

void Cookie::dump() const
{
	std::cout << "Cookie: " << std::endl;
	std::cout << "	Name: " << _name << std::endl;
	std::cout << "	Value: " << _value << std::endl;
	std::cout << "	max_age: " << _maxAge << std::endl;
	std::cout << "	Path: " << _path << std::endl;
	std::cout << "	Domain: " << _domain << std::endl;
	std::cout << "	Secure: " << _secure << std::endl;
	std::cout << "	Httponly: " << _httponly << std::endl;
}

std::string Cookie::getCookie() const
{
	std::stringstream ss;
	ss << _name << "=" << _value << "; ";
	ss << "Max-Age=" << _maxAge << "; ";
	ss << "Path=" << _path << "; ";
	ss << "Domain=" << _domain;
	if (_secure != "")
		ss << "; " << _secure;
	if (_httponly != "")
		ss << "; " << _httponly;
	ss << std::endl;
	return ss.str();
}