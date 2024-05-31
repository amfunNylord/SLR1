#include <fstream>
#include "SLRProgram.h"
#include "SLRGenerator.h"

void SLRProgram::Run(std::ifstream& inputFile, std::ofstream& outputFile)
{
	SLRGenerator generator(inputFile);

	generator.FillTable();

	generator.PrintTable(outputFile);
}
