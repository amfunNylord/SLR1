#include <fstream>
#include "SLRProgram.h"
#include "SLRGenerator.h"
#include "SLRSyntaxParser.h"

void SLRProgram::Run(std::ifstream& inputFile, std::ofstream& outputFile, const std::string& inputChain)
{
	// �������������� slr ���������
	SLRGenerator generator(inputFile);
	// ��������� �������
	generator.FillTable();
	// �������� �������
	generator.PrintTable(outputFile);

	// �������������� �������
	SLRSyntaxParser parser(generator);
	// ����������� �� ������� �������
	parser.ParseChain(inputChain);
}
