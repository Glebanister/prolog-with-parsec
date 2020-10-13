#include "PrologChecker.hpp"

#include <algorithm>

namespace prolog
{
#define OBJECT_ACCUMULATOR(type)                                                            \
    class Accumulator##type : public Parsnip::Accumulator<PrologObjectPtr, PrologObjectPtr> \
    {                                                                                       \
    public:                                                                                 \
        Accumulator##type(const std::string &type)                                          \
            : object(new PrologObject("", type)) {}                                         \
                                                                                            \
        virtual void accum(const PrologObjectPtr &object)                                   \
        {                                                                                   \
            object->objects.push_back(object);                                              \
        }                                                                                   \
                                                                                            \
        virtual PrologObjectPtr result()                                                    \
        {                                                                                   \
            return object;                                                                  \
        }                                                                                   \
                                                                                            \
    private:                                                                                \
        PrologObjectPtr object;                                                             \
    };

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
OBJECT_FROM_STRING_MAKER(ModuleDecl);
OBJECT_MAKER(Disj);
OBJECT_MAKER(Conj);
OBJECT_MAKER(Decl);
OBJECT_MAKER(Atom);
OBJECT_MAKER(AtomSeq);
OBJECT_MAKER(Program);

OBJECT_ACCUMULATOR(RelationSequence);

ParseResult parseProgram(const std::string &text)
{
    using std::string;
    using namespace prolog;
    using namespace Parsnip;
    using PrologObjectParser = yasper::ptr<Parsnip::IParser<std::string, PrologObjectPtr>>;
    using obj = PrologObjectPtr;

    auto undefined = []() { return lazy<std::string, PrologObjectPtr>(); };
    auto ch_tok = [](char c) { return token(skip_ch(c)); };
    auto str_tok = [](std::string s) { return token(skip_str(std::move(s))); };

    // Tokens
    StrParser identifierStringParser = (letter | ch('_')) + optional(many1(letter | digit | ch('_')));
    PrologObjectParser identifier = token(call1(makeFromStringIdentifier, identifierStringParser));
    auto opn = ch_tok('('),
         cls = ch_tok(')'),
         corkscrew = str_tok(":-"),
         period = ch_tok('.'),
         module = str_tok("module");

    // Global parsers
    PrologObjectParser
        program = undefined(),
        expression = undefined(),
        relationDeclaration = undefined(),
        atom = undefined(),
        moduleDeclaration = undefined();

    {
        // Program parsers

        PrologObjectParser
            relationDeclarationSequence = undefined();

        setLazy(relationDeclarationSequence, many<AccumulatorRelationSequence>(relationDeclaration));

        setLazy(program, call2(makeProgram<obj, obj>, moduleDeclaration >> relationDeclarationSequence));
    }

    {
        // Expression parsers
        PrologObjectParser
            expressionTerm = undefined(),
            expressionOperation = undefined(),
            _operators = undefined();

        _operators = op_table(expressionTerm)
                         ->infix_right(";", 10, makeDisj<obj, obj>)
                         ->infix_right(",", 20, makeConj<obj, obj>);

        setLazy(expressionTerm, atom | opn >> expressionOperation >> cls | opn >> expressionTerm >> cls);
        setLazy(expressionOperation, _operators);
        setLazy(expression, expressionOperation | expressionTerm);
    }
    {
        // Declaration parser
        setLazy(relationDeclaration,
                call1(makeDecl<obj>, atom >> period) |
                    call2(makeDecl<obj, obj>, atom >> corkscrew >> expression >> period));
    }
    {
        // Atom parsers
        PrologObjectParser
            atomSeq = undefined(),
            atomBrackets = undefined();

        setLazy(atom, call2(makeAtom<obj, obj>, identifier >> atomSeq));
        setLazy(atomSeq, call2(makeAtomSeq<obj, obj>, opn >> atomBrackets >> cls >> atomSeq) |
                             optional<string, obj>(atom, makeAtom<>()));
        setLazy(atomBrackets, opn >> atomBrackets >> cls | atom);
    }
    {
        // Module parser
        setLazy(moduleDeclaration, call1(makeFromStringModuleDecl, module >> identifierStringParser >> period));
    }
    return Parsnip::parse(text, program);
}

ParsingResultPrinter::ParsingResultPrinter(ParseResult result, const std::string &text)
    : result(result), text(text) {}

std::ostream &operator<<(std::ostream &os, const ParsingResultPrinter &printer)
{
    if (printer.result.parse_finished())
    {
        os << prolog::PrologObjectPrinter(0, "│ ", "├─ ", printer.result.data());
        return os;
    }

    if (printer.result.input_consumed())
    {
        os << "Error: "
           << "unexpected end of input\n";
        return os;
    }
    else
    {
        int badStringEnd = printer.text.find('\n', printer.result.parse_position());
        if (badStringEnd == -1)
        {
            badStringEnd = printer.text.size();
        }
        int badStringBegin = badStringEnd - 1;
        for (; badStringBegin >= 1 && printer.text[badStringBegin] != '\n'; --badStringBegin)
            ;
        int badStringLineN = std::count(printer.text.begin() + badStringBegin, printer.text.begin() + badStringEnd, '\n');
        int badCharPosition = printer.result.parse_position() - badStringBegin;

        for (int i = badStringBegin; i < badStringEnd; ++i)
        {
            os << printer.text[i];
        }
        os << '\n';

        for (int i = 0; i < badStringEnd - badStringBegin; ++i)
        {
            os << (printer.result.parse_position() == i ? "^" : "_");
        }
        os << "\nError: "
           << "unexpected character '"
           << printer.text[printer.result.parse_position()]
           << "' at position "
           << badStringLineN << ':' << badCharPosition
           << std::endl;
    }

    return os;
}
} // namespace prolog
