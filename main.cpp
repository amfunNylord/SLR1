#include <iostream>
#include <fstream>
#include <stdexcept>

#include "SLRProgram.h"
#include <string>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    std::ifstream inputFile((argc == 1) ? "INPUT.TXT" : argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "Can't open input file" << std::endl;
        return 1;
    }

    std::ofstream outputFile("OUTPUT.CSV");

    SLRProgram program;

    std::cout << "Enter input symbol chain: ";
	std::string inputChain;
	std::getline(std::cin, inputChain);

    try 
    {
        program.Run(inputFile, outputFile, inputChain);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
