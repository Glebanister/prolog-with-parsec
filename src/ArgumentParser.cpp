#include "ArgumentParser.hpp"

#include <unordered_map>

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
        "i,input", "Specify input file", cxxopts::value(inputFilename))(
        "atom", "Parse input as atom", cxxopts::value(parsers[ATOM]))(
        "typeexpr", "Parse input as typeexpr", cxxopts::value(parsers[TYPEEXPR]))(
        "type", "Parse input as type declaration", cxxopts::value(parsers[TYPE]))(
        "module", "Parse input as module declaration", cxxopts::value(parsers[MODULE]))(
        "relation", "Parse input as relation", cxxopts::value(parsers[RELATION]))(
        "list", "Parse input as list", cxxopts::value(parsers[LIST]))(
        "prog", "Parse input as program", cxxopts::value(parsers[PROG])->default_value("true"));
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
    whatToParse = static_cast<parserType>(std::distance(parsers.begin(), std::find(parsers.begin(), parsers.end(), true)));
}
} // namespace prolog
