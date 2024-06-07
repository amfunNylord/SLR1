#include "SLRSyntaxParser.h"
#include <sstream>
#include "utils.h"
#include <iostream>
#include <algorithm>

SLRSyntaxParser::SLRSyntaxParser(const SLRGenerator& gen)
{
	m_table = gen.GetTable();
	m_grammar = gen.GetGrammar();
}

void SLRSyntaxParser::ParseChain(const std::string& inputChain)
{
	m_chain = GetConvertedChain(inputChain);

	if (m_chain.empty())
	{
		std::cout << "ERROR" << std::endl;
		return;
	}

	while (true)
	{
		size_t rowNumber = m_stack.empty() ? GetRowOfEl("begin") : GetRowOfEl(m_stack.top());

		std::string stackEl = GetConvertChainEl(m_chain[0], rowNumber);

		if (stackEl == "ok" && m_stack.empty())
		{
			if (m_chain[0] == m_grammar[0].first && m_chain[1] == "=" && m_chain.size() == 2)
			{
				std::cout << "OK" << std::endl;
				break;
			}
			std::cout << "ERROR" << std::endl;
			break;
		}


		if (stackEl.empty())
		{
			if (IsCouldAddNoterminal(rowNumber))
			{
				continue;
			}
			std::cout << "ERROR" << std::endl;
			break;
		}

		if (stackEl[0] == 'R')
		{
			size_t ruleNumber = size_t(std::stoi(stackEl.substr(1, stackEl.size() - 1)));

			std::string insertingSymbol = m_grammar[ruleNumber].first;

			if (m_grammar[ruleNumber].second[0][0] == 'e')
			{
				m_chain.insert(m_chain.begin(), insertingSymbol);
				continue;
			}

			if (ruleNumber == 0)
			{
				while (!m_stack.empty())
				{
					m_stack.pop();
				}
			}
			else
			{
				size_t countOfDeletingItems = m_grammar[ruleNumber].second.size();

				for (size_t k = 0; k < countOfDeletingItems; k++)
				{
					m_stack.pop();
				}
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

bool SLRSyntaxParser::IsEmptySymbolBefore(const std::string& el)
{
	bool result = false;

	std::string possibleEmptySymbol;

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); j++)
		{
			if (GetElFromGrammar(m_grammar[i].second[j]) == el)
			{
				if (j == 0)
				{
					return result;
				}
				possibleEmptySymbol = m_grammar[i].second[j - 1];
			}
		}
	}

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		if (m_grammar[i].first == GetElFromGrammar(possibleEmptySymbol))
		{
			if (GetElFromGrammar(m_grammar[i].second[0]) == "e")
			{
				result = true;
			}
		}
	}

	return result;
}

void SLRSyntaxParser::AddEmptySymbolWhichBefore(const std::string& el)
{
	std::string possibleEmptySymbol;

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); j++)
		{
			if (GetElFromGrammar(m_grammar[i].second[j]) == el)
			{
				possibleEmptySymbol = m_grammar[i].second[j - 1];
			}
		}
	}

	m_chain.insert(m_chain.begin(), GetElFromGrammar(possibleEmptySymbol));
}

bool SLRSyntaxParser::IsCouldAddNoterminal(const size_t& rowNumber)
{
	for (size_t i = 1; i < m_table[rowNumber].size(); i++)
	{
		if (!m_table[rowNumber][i].empty() && m_table[rowNumber][i][0] == 'R' && IsNonTerminal(m_table[0][i]))
		{
			m_chain.insert(m_chain.begin(), m_table[0][i]);
			return true;
		}
	}
	return false;
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