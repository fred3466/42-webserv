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

	std::string strUpperCase(std::string s);
	std::string fromListToString(std::list<std::string> l);
	bool isSpace(char c);
	bool isalnum(std::string s);
	std::string normalizeSpaces(std::string s);
	bool isCommented(std::string s);
	std::string rtrim(std::string &s);
	std::string ltrim(std::string &s);
	std::string trim(std::string &s);
	std::vector<std::string> tokenize(std::string s);
	std::vector<std::string> tokenize(std::string s, char sep);
	std::vector<std::string> tokenize(std::string s, char sep, int nbSeparatorsToProcessParam);
	std::string getNthTokenIfExists(std::vector<std::string> v, int index,
			std::string defaultValue);
	std::string dedoublonne(std::string s, std::string cherche);

};
