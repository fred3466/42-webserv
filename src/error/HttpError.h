#pragma once
#include <iostream>
#include <sstream>

class HttpError
{
private:
	int code;
	std::string description;

public:
	HttpError(int code, const std::string &description);
	~HttpError();

	void setCode(int newCode);
	void setDescription(const std::string &newDescription);

	int getCode() const;
	std::string getDescription() const;
	std::string getStatusLine() const;
};
