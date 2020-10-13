#include "PrologObject.hpp"

namespace prolog
{
PrologObject::PrologObject(std::string code, std::string type)
    : code(std::move(code)), type(std::move(type)) {}
} // namespace prolog
