#include "Parsly.h"

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

using namespace Parsly;

int main(int argc, char **argv)
{
    if (argc < 2)
        return -1;
    std::ifstream ifs(argv[1]);
    std::string inputString((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    inputString.erase(inputString.begin() + 100001, inputString.end());
    std::cout << inputString.size() << std::endl;
    auto t = Char('t');

    auto result = t(inputString);
    while (result and !std::get<1>(*result).empty())
    {
        result = t(std::get<1>(*result));
    }

    if (result)
        std::cout << std::get<0>(*result) << std::endl
                  << std::get<1>(*result) << std::endl;
    else
        std::cout << "no match" << std::endl;
}
