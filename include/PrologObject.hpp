#pragma once

#include <vector>

#include "Parsnip.h"

namespace prolog
{
struct PrologObject
{
    PrologObject(std::string code = "", std::string type = "");
    std::string code;
    std::string type;
    std::vector<ptr<PrologObject>> objects;
};
using PrologObjectPtr = ptr<PrologObject>;
} // namespace prolog
