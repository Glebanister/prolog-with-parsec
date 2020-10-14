#include <fstream>
#include <iostream>
#include <string>

#include "ArgumentParser.hpp"
#include "PrologChecker.hpp"

int main(int argc, const char **argv)
{
    try
    {
        prolog::PrologCheckerArgumentParser args(argc, argv);
        std::ifstream inputFile(args.inputFilename);
        if (!inputFile.good())
        {
            throw std::invalid_argument("Can not read from '" + args.inputFilename + "'");
        }
        std::string programText{std::istreambuf_iterator<char>(inputFile),
                                std::istreambuf_iterator<char>()};
        std::ofstream outputFile(args.inputFilename + ".out");
        if (!outputFile.good())
        {
            throw std::invalid_argument("Can not write to '" + args.inputFilename + ".out'");
        }
        outputFile << prolog::ParsingResultPrinter(prolog::parseProgram(programText, args.whatToParse), programText);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
