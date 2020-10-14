#include "ArgumentParser.hpp"

namespace prolog
{
PrologCheckerArgumentParser::PrologCheckerArgumentParser(int argc, const char **argv)
    : options_("prolog-checker",
               ""
               "Prolog grammar checker\n"
               "Specify input file path, checker will put error information to stdout")
{
    options_.add_options()(
        "h,help", "Print usage")(
        "i,input", "Specify input file", cxxopts::value(inputFilename));
    auto result = options_.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options_.help() << std::endl;
        exit(0);
    }
    if (!result.count("input"))
    {
        throw cxxopts::option_required_exception("input");
    }
}
} // namespace prolog
