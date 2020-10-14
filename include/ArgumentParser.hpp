#pragma once

#include <array>

#include "PrologChecker.hpp"
#include "cxxopts.hpp"

namespace prolog
{
struct PrologCheckerArgumentParser
{
    PrologCheckerArgumentParser(int argc, const char **argv);

    std::string inputFilename;
    parserType whatToParse;

private:
    cxxopts::Options options_;

    std::array<bool, parserType::_TOTAL> parsers{};
};
} // namespace prolog
