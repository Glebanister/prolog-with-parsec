#include <ostream>

#include "PrologObjectPrinter.hpp"

namespace prolog
{
PrologObjectPrinter::PrologObjectPrinter(std::size_t indentationLevel,
                                         std::string indentationHorizontal,
                                         std::string indentationVertical,
                                         PrologObjectPtr obj)
    : indentLevel(indentationLevel),
      indentationHorizontal(std::move(indentationHorizontal)),
      indentationVertical(std::move(indentationVertical)),
      object(obj)
{
}

std::ostream &operator<<(std::ostream &os, const PrologObjectPrinter &printer)
{
    for (std::size_t i = 0; i < printer.indentLevel; ++i)
    {
        os << printer.indentationHorizontal;
    }
    os << printer.indentationVertical;
    os << printer.object->type << (printer.object->code.empty() ? "" : (": '" + printer.object->code + "'"));
    os << '\n';
    for (const auto subobj : printer.object->objects)
    {
        os << PrologObjectPrinter(printer.indentLevel + 1, printer.indentationHorizontal, printer.indentationVertical, subobj);
    }
    return os;
}
} // namespace prolog
