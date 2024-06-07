#pragma once
#include "SLRGenerator.h"
#include <stack>

class SLRSyntaxParser
{
public:
	SLRSyntaxParser(const SLRGenerator& gen);

	void ParseChain(const std::string& inputChain);

private:
	std::vector<std::string> GetConvertedChain(const std::string& chain);
	std::string GetConvertChainEl(const std::string& el, const size_t& rowNumber);

	bool IsEmptySymbolBefore(const std::string& el);

	void AddEmptySymbolWhichBefore(const std::string& el);

	void TryToConvert();

	size_t GetRowOfEl(const std::string& el);

	std::vector<std::vector<std::string>> m_table;
	std::vector<std::pair<std::string, std::vector<std::string>>> m_grammar;

	std::stack<std::string> m_stack;
	std::vector<std::string> m_chain;
};
