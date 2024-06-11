#include <fstream>
#include "SLRProgram.h"
#include "SLRGenerator.h"
#include "SLRSyntaxParser.h"

void SLRProgram::Run(std::ifstream& inputFile, std::ofstream& outputFile, const std::string& inputChain)
{
	// инициализируем slr генератор
	SLRGenerator generator(inputFile);
	// заполняем таблицу
	generator.FillTable();
	// печатаем таблицу
	generator.PrintTable(outputFile);

	// инициализируем бегунок
	SLRSyntaxParser parser(generator);
	// пробегаемся по входной цепочке
	parser.ParseChain(inputChain);
}
