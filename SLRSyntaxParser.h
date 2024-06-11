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

	// проверяем может ли быть пустой символ перед данным элементом
	bool IsEmptySymbolBefore(const std::string& el);

	// добавляем в входную цепочку символ, который может быть пустым
	void AddEmptySymbolWhichBefore(const std::string& el);
	
	// проверяем можно ли добавить нетерминал
	bool IsCouldAddNoterminal(const size_t& rowNumber);

	std::string RollUp(const size_t& rowNumber);

	bool IsCouldBeEmpty(const std::string& el);

	size_t GetRowOfEl(const std::string& el);

	std::vector<std::vector<std::string>> m_table;
	std::vector<std::pair<std::string, std::vector<std::string>>> m_grammar;

	std::stack<std::string> m_stack;
	std::vector<std::string> m_chain;
};
