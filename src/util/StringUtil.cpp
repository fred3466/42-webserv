#include "StringUtil.h"

StringUtil::StringUtil()
{
}

StringUtil::~StringUtil()
{
}

std::string StringUtil::strUpper(std::string s)
{
	std::string ret = s;
	for (std::string::iterator ite = ret.begin(); ite != ret.end(); ite++)
	{
		*ite = toupper(*ite);
	}
	return ret;
}

std::string StringUtil::fromListToString(std::list<std::string> l)
{
	std::string ret;
	for (std::list<std::string>::iterator ite = l.begin(); ite != l.end();
			ite++)
	{
		ret += *ite;
	}
	return ret;
}

//supprime les espaces en fin de chaine
void StringUtil::rtrim(std::string &s)
{
	if (s.empty())
		return;

	std::string::iterator p;
	for (p = s.end(); p != s.begin() && *--p == ' ';)
		;

	if (*p != ' ')
		p++;

	if (p != s.end())
		s.erase(p, s.end());
}

//supprime les espaces en début de chaine
void StringUtil::ltrim(std::string &s)
{
	if (s.empty())
		return;

	std::string::iterator p;
	for (p = s.begin(); p != s.end() && *p == ' ';)
		p++;

	if (p != s.begin())
		s.erase(s.begin(), p);
}

//supprime les espaces en début et fin de chaine
void StringUtil::trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}

bool StringUtil::isSpace(char c)
{
	return std::isspace(c);
}

bool StringUtil::isCommented(std::string s)
{
	bool bMoreSpaces = true;
	int i;
	for (i = 0; bMoreSpaces && (i < s.length()); i++)
	{
		bMoreSpaces = isSpace(s[i]);
	}
	if (i > 0 && s[i - 1] == '#')
		return true;
	return false;
}

bool StringUtil::isalnum(std::string s)
{
	bool ret = true;
	int i = 0;
	const char *cstr = s.c_str();
	while (ret && std::isalnum(cstr[i]))
		i++;
	return i == s.length();
}

std::string StringUtil::normalizeSpaces(std::string s)
{
	int count = 0;
	std::string ret;
	char *cstrWithNoTab = new char[s.length()];
	memccpy(cstrWithNoTab, s.c_str(), 0, s.length());

	for (int i = 0; i < s.length(); i++)
	{
		bool bIsSpaces = isSpace(cstrWithNoTab[i]) && cstrWithNoTab[i] != ' ';
		if (bIsSpaces)
			cstrWithNoTab[i] = ' ';
	}
	if (isSpace(cstrWithNoTab[s.length() - 1])
			&& cstrWithNoTab[s.length() - 1] != ' ')
		cstrWithNoTab[s.length() - 1] = ' ';

	for (int i = 0; i < s.length(); i++)
	{
		bool bTwosSpaces = isSpace(cstrWithNoTab[i]) && i > 0
				&& isSpace(cstrWithNoTab[i - 1]);
		if (!bTwosSpaces && i > 0)
			ret += cstrWithNoTab[i - 1];
	}
	if (!isSpace(cstrWithNoTab[s.length() - 1]))
		ret += cstrWithNoTab[s.length() - 1];
	return ret;
}

std::vector<std::string> StringUtil::tokenize(std::string s)
{
	std::vector<std::string> ret = std::vector<std::string>();
	std::stringstream ss(s);
	std::string word;
	while (ss >> word)
	{
		ret.push_back(word);
	}
	return ret;
}

std::vector<std::string> StringUtil::tokenize(std::string s, char sep)
{
	std::vector<std::string> ret = std::vector<std::string>();
	std::stringstream ss(s);
	std::string word;
	while (!ss.eof())
	{
		getline(ss, word, sep);
		ret.push_back(word);
	}
	return ret;
}

std::string StringUtil::getNthTokenIfExists(std::vector<std::string> v,
		int index,
		std::string defaultValue)
{
	if (v.size() > index)
		return v[index];
	else
		return defaultValue;
}

