#pragma once

#include <string>

#include "PrologObject.hpp"

namespace prolog
{
struct PrologObjectPrinter
{
    PrologObjectPrinter(std::size_t indentationLevel,
                        std::string indentationHorizontal,
                        std::string indentationVertical,
                        PrologObjectPtr obj);
    std::size_t indentLevel = 0;
    std::string indentationHorizontal;
    std::string indentationVertical;
    PrologObjectPtr object;
};

std::ostream &operator<<(std::ostream &os, const PrologObjectPrinter &printer);
} // namespace prolog
