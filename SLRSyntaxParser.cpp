#include "SLRSyntaxParser.h"
#include <sstream>
#include "utils.h"
#include <iostream>

SLRSyntaxParser::SLRSyntaxParser(const SLRGenerator& gen)
{
	m_table = gen.GetTable();
	m_grammar = gen.GetGrammar();
}

void SLRSyntaxParser::ParseChain(const std::string& inputChain)
{
	m_chain = GetConvertedChain(inputChain);

	while (true)
	{
		size_t rowNumber = m_stack.empty() ? GetRowOfEl("begin") : GetRowOfEl(m_stack.top());
	
		std::string stackEl = GetConvertChainEl(m_chain[0], rowNumber);

		if (stackEl == "ok" && m_stack.empty())
		{
			std::cout << "The input program is ok" << std::endl;
			break;
		}

		if (stackEl[0] == 'R')
		{
			size_t ruleNumber = size_t(std::stoi(stackEl.substr(1, stackEl.size() - 1)));

			if (ruleNumber == 0)
			{
				m_stack.push("=02");
			}

			std::string insertingSymbol = m_grammar[ruleNumber].first;

			if (m_grammar[ruleNumber].second[0][0] == 'e')
			{
				m_chain.insert(m_chain.begin(), insertingSymbol);
				continue;
			}

			size_t countOfDeletingItems = m_grammar[ruleNumber].second.size();

			for (size_t k = 0; k < countOfDeletingItems; k++)
			{
				m_stack.pop();
			}

			m_chain.insert(m_chain.begin(), insertingSymbol);
		}
		else
		{
			m_stack.push(stackEl);
			m_chain.erase(m_chain.begin());
		}
	}
}

std::vector<std::string> SLRSyntaxParser::GetConvertedChain(const std::string& chain)
{
	std::vector<std::string> result;
	std::stringstream ss(chain);
	std::string terminal;
	while (ss >> terminal)
	{
		result.emplace_back(terminal);
	}

	return result;
}

std::string SLRSyntaxParser::GetConvertChainEl(const std::string& el, const size_t& rowNumber)
{
	for (size_t i = 0; i < m_table[rowNumber].size(); i++)
	{
		if (m_table[0][i] == el)
		{
			return m_table[rowNumber][i];
		}
	}
	return std::string();
}

size_t SLRSyntaxParser::GetRowOfEl(const std::string& el)
{
	for (size_t i = 1; i < m_table.size(); i++)
	{
		if (m_table[i][0] == el)
		{
			return i;
		}
	}
	return size_t();
}