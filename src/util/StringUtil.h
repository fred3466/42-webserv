#pragma once
#include <cstring>
#include <string>
#include <list>
#include <cctype>
#include <vector>
#include <fstream>
#include <sstream>

class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	std::string strUpper(std::string s);
	std::string fromListToString(std::list<std::string> l);
	bool isSpace(char c);
	bool isalnum(std::string s);
	std::string normalizeSpaces(std::string s);
	bool isCommented(std::string s);
	void rtrim(std::string &s);
	void ltrim(std::string &s);
	void trim(std::string &s);
	std::vector<std::string> tokenize(std::string s);
	std::vector<std::string> tokenize(std::string s, char sep);
	std::string getNthTokenIfExists(std::vector<std::string> v, int index,
			std::string defaultValue);
	std::string dedoublonne(std::string s, std::string cherche);

};
