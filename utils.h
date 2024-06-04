#pragma once

#include <algorithm>
#include <string>

inline void Trim(std::string& str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));

	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}).base(), str.end());
}

inline bool IsNonTerminal(const std::string& el)
{
	return (el.size() >= 2 && el[0] == '<' && el[el.size() - 1] == '>');
}

inline std::string GetElFromGrammar(const std::string& nonTerminal)
{
	std::string res;

	size_t i = 0;
	while (i < nonTerminal.size())
	{
		if (isdigit(nonTerminal[i]))
		{
			break;
		}
		res += nonTerminal[i];
		i++;
	}
	return res;
}