#include <iostream>
#include <fstream>
#include <stdexcept>

#include "SLRProgram.h"

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

    // нельзя использовать - терминалы цифры
    try 
    {
        program.Run(inputFile, outputFile);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "The program is done" << std::endl;

    return 0;
}
