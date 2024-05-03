#pragma once
#include <cstring>
#include <string>
#include <list>
#include <cctype>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	std::string strUpperCase(std::string s);
	std::string fromListToString(std::list<std::string> *l, std::string endLine);
	std::string fromCArrayToString(const char *a[]);
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
	bool isStrictlyEqual(std::string s1, std::string s2);
	std::string strFromInt(int i);
	int intFromStr(std::string s);
	std::string formatDate(time_t mtime, std::string strFormat);
	std::string toHexa(int i);
	std::string getAliasFromVal(const std::string &val);
	std::string replace_all(std::string &s, std::string const &toReplace, std::string const &replaceWith);
	size_t findStringInCString(char *cString, int cStringLen, std::string toFind, int offset);
};
