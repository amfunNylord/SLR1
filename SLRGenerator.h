#pragma once

#include <vector>
#include <set>
#include <fstream>

class SLRGenerator
{
public:
	SLRGenerator(std::ifstream& inputFile);
	void FillTable();
	void PrintTable(std::ofstream& outputFile);

private:
	void AddStartingRule();
	void AddNewRow();
	void AddNewFilledLine(const std::string& el);

	bool IsElAlreadyInTable(const std::string& el);

	std::string GetElFromGrammar(const std::string& nonTerminal);
	std::string GetPuttingEl(const std::string& el);
	std::vector<std::string> GetFirstFollowSet(const std::string& nonterminal);
	std::vector<std::string> GetSymbolsFromEl(const std::string& el);
	std::vector<std::string> GetTableContent(const std::string& el);
	std::vector<std::string> GetNextContentAfterLastInLine(const std::string& el);

	std::vector<std::vector<std::string>> m_table;

	std::vector<std::string> m_uniqueSymbols;

	size_t m_tableWidth;
	std::vector<std::pair<std::string, std::vector<std::string>>> m_grammar;
	
	std::vector<std::string> m_nonTerminals;
	std::vector<std::string> m_rightSideRules;
	std::vector<std::set<std::string>> m_sets;
};
