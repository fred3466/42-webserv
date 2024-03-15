#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

class Cookie
{
private:
	long _maxAge;
	std::string _path;
	std::string _domain;
	std::string _secure;
	std::string _httponly;
	std::string _name;
	std::string _value;
public:
	Cookie();
	~Cookie();
	long getMaxAge() const;
	std::string getPath() const;
	std::string getDomain() const;
	std::string getSecure() const;
	std::string getHttponly() const;
	std::string getName() const;
	std::string getValue() const;
	void setMaxAge(long &max_age);
	void setPath(const std::string &path);
	void setDomain(const std::string &domain);
	void setSecure();
	void setHttponly();
	void setName(const std::string &name);
	void setValue(const std::string &value);
	std::string getCookie() const;
	void dump() const;
};