#pragma once

#include "PrologObject.hpp"
#include "PrologObjectPrinter.hpp"

namespace prolog
{
#define OBJECT_MAKER(name)                          \
    template <typename... Args>                     \
    inline PrologObjectPtr make##name(Args... args) \
    {                                               \
        auto obj = new PrologObject("", #name);     \
        (obj->objects.push_back(args), ...);        \
        return obj;                                 \
    }

#define OBJECT_FROM_STRING_MAKER(name)                         \
    inline PrologObjectPtr makeFromString##name(std::string s) \
    {                                                          \
        auto obj = new PrologObject(s, #name);                 \
        return obj;                                            \
    }

OBJECT_FROM_STRING_MAKER(Identifier);
OBJECT_MAKER(Disj);
OBJECT_MAKER(Conj);
OBJECT_MAKER(Decl);
OBJECT_MAKER(Atom);
OBJECT_MAKER(AtomSeq);
OBJECT_FROM_STRING_MAKER(ModuleDecl);
} // namespace prolog
