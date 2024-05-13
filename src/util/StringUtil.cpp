#include "StringUtil.h"

StringUtil::StringUtil()
{
}

StringUtil::~StringUtil()
{
}

std::string StringUtil::strUpperCase(std::string s)
{
	std::string ret = s;
	for (std::string::iterator ite = ret.begin(); ite != ret.end(); ite++)
	{
		*ite = toupper(*ite);
	}
	return ret;
}

std::string StringUtil::fromListToString(std::list<std::string> *l, std::string endLine)
{
	std::string ret;
	for (std::list<std::string>::iterator ite = l->begin(); ite != l->end();
			ite++)
	{
		ret += *ite + endLine;
	}
	return ret;
}
std::string StringUtil::fromCArrayToString(const char *a[])
{
	std::string ret;
	bool bIsFirst = true;
	while (*a)
	{
		if (!bIsFirst)
		{
			ret += " ";
		} else
		{
			bIsFirst = false;
		}
		ret += *a;
		a++;
	}
	return ret;
}

// supprime les espaces en fin de chaine
std::string StringUtil::rtrim(std::string &s)
{
	if (s.empty())
		return "";
	;

	std::string::iterator p;
	for (p = s.end() - 1; p != s.begin() && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r');)
		p--;

	if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && *p != '\0')
		p++;

	if (p != s.end())
		s.erase(p, s.end());
	return s;
}

// supprime les espaces en début de chaine
std::string StringUtil::ltrim(std::string &s)
{
	if (s.empty())
		return "";

	std::string::iterator p;
	for (p = s.begin(); p != s.end() && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'); p++)
		;
	//			p++;

	//	if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
	//		p--;

	if (p != s.begin())
		s.erase(s.begin(), p);
	return s;
}

// supprime les espaces en début et fin de chaine
std::string StringUtil::trim(std::string &s)
{
	std::string rtrimmed = rtrim(s);
	return ltrim(rtrimmed);
}

bool StringUtil::isSpace(char c)
{
	return std::isspace(c);
}

bool StringUtil::isCommented(std::string s)
{
	bool bMoreSpaces = true;
	int i;
	for (i = 0; bMoreSpaces && (i < (int) s.length()); i++)
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
	return i == (int) s.length();
}

std::string StringUtil::dedoublonne(std::string s, std::string cherche)
{
	int chercheLen = cherche.length();
	for (size_t ite = s.find(cherche + cherche); ite != std::string::npos; ite++)
	{
		s.erase(ite, chercheLen);
	}
	return s;
}
std::string StringUtil::normalizeSpaces(std::string s)
{
	std::string ret;
	char *cstrWithNoTab = new char[s.length()];
	memccpy(cstrWithNoTab, s.c_str(), 0, s.length());

	for (int i = 0; i < (int) s.length(); i++)
	{
		bool bIsSpaces = isSpace(cstrWithNoTab[i]) && cstrWithNoTab[i] != ' ';
		if (bIsSpaces)
			cstrWithNoTab[i] = ' ';
	}
	if (isSpace(cstrWithNoTab[s.length() - 1]) && cstrWithNoTab[s.length() - 1] != ' ')
		cstrWithNoTab[s.length() - 1] = ' ';

	for (int i = 0; i < (int) s.length(); i++)
	{
		bool bTwosSpaces = isSpace(cstrWithNoTab[i]) && i > 0 && isSpace(cstrWithNoTab[i - 1]);
		if (!bTwosSpaces && i > 0)
			ret += cstrWithNoTab[i - 1];
	}
	if (!isSpace(cstrWithNoTab[s.length() - 1]))
		ret += cstrWithNoTab[s.length() - 1];
	delete[] cstrWithNoTab;
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
	return tokenize(s, sep, -1);
}

std::vector<std::string> StringUtil::tokenize(std::string s, char sep, int nbSeparatorsToProcessParam)
{
	std::vector<std::string> ret = std::vector<std::string>();
	std::stringstream ss(s);
	std::string word;
	if (nbSeparatorsToProcessParam < 1)
	{
		while (!ss.eof())
		{
			getline(ss, word, sep);
			ret.push_back(word);
		}
	}
	else
	{
		int nbSeparatorsToProcess = nbSeparatorsToProcessParam;
		while (!ss.eof() && nbSeparatorsToProcess--)
		{
			getline(ss, word, sep);
			ret.push_back(word);
		}
		getline(ss, word);
		ret.push_back(word);
	}

	return ret;
}

const std::string StringUtil::getNthTokenIfExists(std::vector<std::string> v, int index, std::string defaultValue)
{
	std::string ret = defaultValue;
	if (index < (int) v.size())
	{
		ret = (v.at(index));
	}
	return ret;
}

bool StringUtil::isStrictlyEqual(std::string s1, std::string s2)
{
	return strUpperCase(s1) == strUpperCase(s2);
}

std::string StringUtil::strFromInt(int i)
{
	std::string ret;
	std::stringstream ss;
	ss << i;
	ss >> ret;

	return ret;
}
int StringUtil::intFromStr(std::string s)
{
	std::string trimmed = trim(s);
	if (trimmed == "")
	{
		return 0;
	}

	std::stringstream ss(trimmed);
	int resInt;
	ss >> resInt;

	return resInt;
}

std::string StringUtil::formatDate(time_t mtime, std::string strFormat)
{
	char buf[1000];
	struct tm tmm = *gmtime(&mtime);
	strftime(buf, sizeof buf, strFormat.c_str(), &tmm);
	std::string ret = std::string(buf);
	return ret;
}

std::string StringUtil::toHexa(int i)
{
	std::string ret = "";
	std::stringstream stream;
	stream << std::hex << i;
	stream >> ret;

	return ret;
}

std::string StringUtil::replace_all(std::string &s, std::string const &toReplace, std::string const &replaceWith)
{
	std::string buf;
	std::size_t pos = 0;
	std::size_t prevPos;

	buf.reserve(s.size());

	while (true)
	{
		prevPos = pos;
		pos = s.find(toReplace, pos);
		if (pos == std::string::npos)
			break;
		buf.append(s, prevPos, pos - prevPos);
		buf += replaceWith;
		pos += toReplace.size();
	}

	buf.append(s, prevPos, s.size() - prevPos);
	s.swap(buf);
	return s;
}

std::string StringUtil::getAliasFromVal(const std::string &val)
{
	std::vector<std::string> tokens = tokenize(val, ' ');
	if (tokens.size() >= 2)
	{
		return tokens[1];
	}
	return "";
}

size_t StringUtil::findStringInCString(char *cString, int cStringLen, std::string toFind, int offset)
{
	const char *toFindCString = toFind.c_str();
	char *cStringOfsset = cString + offset;
	int i = 0;
	for (i = 0; i < cStringLen; i++)
	{
		bool found = false;
		if (cStringOfsset[i] == toFindCString[0])
		{
			found = true;
			for (size_t j = 1; j < toFind.length(); j++)
			{
				if (cStringOfsset[i + j] != toFindCString[j])
				{
					found = false;
					break;
				}
				found = true;
			}

		}

		if (found)
			return offset + i;
	}
	return -1;
}
