#include "SLRGenerator.h"
#include <sstream>
#include <queue>
#include "utils.h"

SLRGenerator::SLRGenerator(std::ifstream& inputFile)
{
	std::string line;
	int row = 0;
	while (std::getline(inputFile, line))
	{
		std::stringstream ss(line);
		std::getline(ss, line, '-');

		Trim(line);
		std::string lhs = line;
		m_nonTerminals.emplace_back(lhs);

		std::getline(ss, line, '/');
		Trim(line);
		m_rightSideRules.emplace_back(line);
		std::vector<std::string> rhs;
		
		std::stringstream sss(line);
		int column = 1;
		while (sss >> line)
		{
			rhs.emplace_back(line + std::to_string(row) + std::to_string(column));
			column++;
		}
		
		m_grammar.insert(m_grammar.end(), std::make_pair(lhs, rhs));

		std::set<std::string> guidingSet;
		while (std::getline(ss, line, ','))
		{
			Trim(line);
			guidingSet.insert(line);
		}
		m_sets.emplace_back(guidingSet);
		row++;
	}

	std::set<std::string> uniqueSymbols;

	auto nonTerminals = m_nonTerminals;
	for (size_t i = 0; i < nonTerminals.size(); i++)
	{
		if (uniqueSymbols.find(nonTerminals[i]) == uniqueSymbols.end())
		{
			uniqueSymbols.insert(nonTerminals[i]);
			m_uniqueSymbols.emplace_back(nonTerminals[i]);
		}
	}
	auto rightSideRules = m_rightSideRules;
	for (size_t i = 0; i < rightSideRules.size(); i++)
	{
		std::stringstream ss(rightSideRules[i]);
		std::string tempLine;
		while (ss >> tempLine)
		{
			if (uniqueSymbols.find(tempLine) == uniqueSymbols.end() && tempLine != "=")
			{
				uniqueSymbols.insert(tempLine);
				m_uniqueSymbols.emplace_back(tempLine);
			}
		}
	}

	m_uniqueSymbols.emplace_back("=");

	m_tableWidth = m_uniqueSymbols.size() + 1;

	AddNewRow();

	for (size_t i = 1; i < m_tableWidth; i++)
	{
		m_table[0][i] = m_uniqueSymbols[i - 1];
	}
}

void SLRGenerator::FillTable()
{
	AddStartingRule();
	std::queue<std::string> q;
	size_t row = 1;
	size_t column = 2;
	while (true)
	{
		if (column == m_tableWidth - 1 && row == m_table.size() - 1)
		{
			break;
		}
		if (q.empty())
		{
			if (!m_table[row][column].empty())
			{
				if (m_table[row][column][0] != 'R')
				{
					q.push(m_table[row][column]);
				}
			}
			if (column + 1 < m_tableWidth)
			{
				column++;
			}
			else
			{
				if (row + 1 < m_table.size())
				{
					row++;
				}
				column = 1;
			}
		}
		else
		{
			std::string el = q.front();
			q.pop();
			if (!IsElAlreadyInTable(el))
			{
				AddNewFilledLine(el);
			}
		}
	}
}

void SLRGenerator::AddStartingRule()
{
	AddNewRow();

	m_table[1][0] = "begin";
	m_table[1][1] = "ok";

	std::string firstSymbol = m_grammar[0].second[0];

	std::string nonTerminal = GetElFromGrammar(firstSymbol);

	std::vector<std::string> firstFollowSet = GetFirstFollowSet(nonTerminal);

	firstFollowSet.insert(firstFollowSet.begin(), firstSymbol);

	for (const auto& e : firstFollowSet)
	{
		std::string el = GetElFromGrammar(e);
		for (size_t i = 1; i < m_tableWidth; i++)
		{
			if (el == m_table[0][i])
			{
				m_table[1][i] += e;
				break;
			}
		}
	}
}

void SLRGenerator::AddNewRow()
{
	m_table.resize(m_table.size() + 1);
	m_table[m_table.size() - 1].resize(m_tableWidth);
}

void SLRGenerator::AddNewFilledLine(const std::string& el)
{
	//todo:
	AddNewRow();
	m_table[m_table.size() - 1][0] = el;

	if (el[0] == '=')
	{
		m_table[m_table.size() - 1][m_tableWidth - 1] = "R0";
		return;
	}

	std::vector<std::string> symbols = GetSymbolsFromEl(el);

	std::vector<std::string> tableContent;
	for (size_t i = 0; i < symbols.size(); i++)
	{
		std::vector<std::string> content = GetTableContent(symbols[i]);
		tableContent.insert(tableContent.end(), content.begin(), content.end());
	}

	// Копирование элементов вектора во множество
	std::set<std::string> uniqueSet(tableContent.begin(), tableContent.end());

	// Копирование уникальных элементов обратно в вектор
	tableContent.assign(uniqueSet.begin(), uniqueSet.end());

	for (size_t i = 0; i < tableContent.size(); i++)
	{
		std::string el = GetElFromGrammar(tableContent[i]);
		for (size_t j = 1; j < m_tableWidth; j++)
		{
			if (m_table[0][j] == el)
			{
				std::string puttingEl = GetPuttingEl(tableContent[i]);
				m_table[m_table.size() - 1][j] += puttingEl;
				break;
			}
		}
	}
}

void SLRGenerator::PrintTable(std::ofstream& outputFile)
{
	for (const auto& row : m_table)
	{
		std::copy(row.begin(), row.end(), std::ostream_iterator<std::string>(outputFile, ";"));
		outputFile << "\n";
	}
}

bool SLRGenerator::IsElAlreadyInTable(const std::string& el)
{
	for (size_t i = 1; i < m_table.size(); i++)
	{
		if (m_table[i][0] == el)
		{
			return true;
		}
	}
	return false;
}

std::string SLRGenerator::GetElFromGrammar(const std::string& nonTerminal)
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

std::string SLRGenerator::GetPuttingEl(const std::string& el)
{
	std::string result;

	size_t i = 0;
	while (!isdigit(el[i]))
	{
		i++;
	}

	while (isdigit(el[i]))
	{
		i++;
	}

	if (i == el.size())
	{
		return el;
	}

	while (i < el.size())
	{
		result += el[i];
		i++;
	}

	return result;
}

std::vector<std::string> SLRGenerator::GetFirstFollowSet(const std::string& nonterminal)
{
	std::vector<std::string> res;

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		if (m_grammar[i].first != nonterminal)
		{
			continue;
		}
		res.emplace_back(m_grammar[i].second[0]);

		std::string el = GetElFromGrammar(m_grammar[i].second[0]);

		if (IsNonTerminal(el))
		{
			if (el == m_grammar[i].first)
			{
				continue;
			}
			std::vector<std::string> res1 = GetFirstFollowSet(el);
			res.insert(res.end(), res1.begin(), res1.end());
		}
	}

	return res;
}

std::vector<std::string> SLRGenerator::GetSymbolsFromEl(const std::string& el)
{
	std::vector<std::string> res;

	size_t i = 0; 
	while (i < el.size())
	{
		std::string symbol;
		while (!isdigit(el[i]))
		{
			symbol += el[i];
			i++;
		}
		while (isdigit(el[i]) && i < el.size())
		{
			symbol += el[i];
			i++;
		}

		res.emplace_back(symbol);
	}

	return res;
}

std::vector<std::string> SLRGenerator::GetTableContent(const std::string& el)
{
	std::vector<std::string> result;

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); j++)
		{
			if (m_grammar[i].second[j] == el)
			{
				if (j + 1 == m_grammar[i].second.size())
				{
					/*if (m_grammar[i].first == GetElFromGrammar(m_grammar[i].second[j]))
					{
						continue;
					}*/
					std::vector<std::string> result1 = GetNextContentAfterLastInLine(m_grammar[i].first);

					for (size_t k = 0; k < result1.size(); k++)
					{
						if (result1[k][result1[k].size() - 1] == 'R')
						{
							result1[k] += std::to_string(i);
						}
					}

					result.insert(result.end(), result1.begin(), result1.end());
				}
				else
				{
					std::string elem = m_grammar[i].second[j + 1];
					if (elem[0] == '=')
					{
						elem += 'R' + std::to_string(i);
					}
					result.emplace_back(elem);
					std::string el = GetElFromGrammar(elem);
					if (IsNonTerminal(el))
					{
						std::vector<std::string> followSet = GetFirstFollowSet(el);
						result.insert(result.begin(), followSet.begin(), followSet.end());
					}
				}
				break;
			}
		}
	}

	return result;
}

std::vector<std::string> SLRGenerator::GetNextContentAfterLastInLine(const std::string& el)
{
	std::vector<std::string> result;

	for (size_t i = 0; i < m_grammar.size(); i++)
	{
		for (size_t j = 0; j < m_grammar[i].second.size(); j++)
		{
			std::string elFromGrammar = GetElFromGrammar(m_grammar[i].second[j]);
			if (elFromGrammar == el)
			{
				if (j + 1 == m_grammar[i].second.size())
				{
					if (m_grammar[i].first == elFromGrammar)
					{
						continue;
					}
					std::vector<std::string> result1 = GetNextContentAfterLastInLine(m_grammar[i].first);
					result.insert(result.end(), result1.begin(), result1.end());
				}
				else
				{
					result.emplace_back(m_grammar[i].second[j + 1] + "R");
				}
				break;
			}
		}
	}

	return result;
}
