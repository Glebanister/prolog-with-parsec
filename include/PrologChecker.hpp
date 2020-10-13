#pragma once

#include <string_view>

#include "PrologObject.hpp"
#include "PrologObjectPrinter.hpp"

namespace prolog
{
using ParseResult = Parsnip::ParseResult<prolog::PrologObjectPtr>;
ParseResult parseProgram(const std::string &program);

struct ParsingResultPrinter
{
    ParsingResultPrinter(ParseResult result, const std::string &text);

    ParseResult result;
    const std::string text;
};

std::ostream &operator<<(std::ostream &os, const ParsingResultPrinter &printer);
} // namespace prolog
