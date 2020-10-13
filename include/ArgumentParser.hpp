#pragma once

#include "cxxopts.hpp"

namespace prolog
{
    struct PrologCheckerArgumentParser
    {
        PrologCheckerArgumentParser(int argc, const char **argv);

        bool isTesting;
        std::string inputFilename;

    private:
        cxxopts::Options options_;
    };
} // namespace prolog
