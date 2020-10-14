#pragma once

#include "cxxopts.hpp"

namespace prolog
{
    struct PrologCheckerArgumentParser
    {
        PrologCheckerArgumentParser(int argc, const char **argv);

        std::string inputFilename;

    private:
        cxxopts::Options options_;
    };
} // namespace prolog
