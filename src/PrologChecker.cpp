#include "PrologChecker.hpp"

#include <algorithm>

namespace prolog
{
#define OBJECT_REDUCER(type)                                                  \
    PrologObjectPtr reduce##type(PrologObjectPtr left, PrologObjectPtr right) \
    {                                                                         \
        PrologObjectPtr reduced = new PrologObject("", #type);                \
        reduced->objects.push_back(left);                                     \
        reduced->objects.push_back(right);                                    \
        return reduced;                                                       \
    }

#define OBJECT_ACCUMULATOR(type)                                                            \
    class Accumulator##type : public Parsnip::Accumulator<PrologObjectPtr, PrologObjectPtr> \
    {                                                                                       \
    public:                                                                                 \
        virtual void accum(const PrologObjectPtr &nextObject)                               \
        {                                                                                   \
            object->objects.push_back(nextObject);                                          \
        }                                                                                   \
                                                                                            \
        virtual PrologObjectPtr result()                                                    \
        {                                                                                   \
            return object;                                                                  \
        }                                                                                   \
                                                                                            \
    private:                                                                                \
        PrologObjectPtr object = new PrologObject("", #type);                               \
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
OBJECT_FROM_STRING_MAKER(Variable);
OBJECT_FROM_STRING_MAKER(ModuleDecl);
OBJECT_FROM_STRING_MAKER(TypeDecl);

OBJECT_MAKER(Disj);
OBJECT_MAKER(Arrow);
OBJECT_MAKER(Conj);
OBJECT_MAKER(Relation);
OBJECT_MAKER(Atom);
OBJECT_MAKER(AtomSeq);
OBJECT_MAKER(Program);
OBJECT_MAKER(TypeDecl);
OBJECT_MAKER(ListItems);
OBJECT_MAKER(List);
OBJECT_MAKER(ListHeadTail);
OBJECT_MAKER(EmptyProgram);
OBJECT_MAKER(EmptyList);

OBJECT_ACCUMULATOR(RelationSequence);
OBJECT_ACCUMULATOR(TypeDeclarationSequence);
OBJECT_ACCUMULATOR(ListItems);
OBJECT_ACCUMULATOR(AtomSequence);

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
    auto
        opn = ch_tok('('),
        cls = ch_tok(')'),
        sqopn = ch_tok('['),
        sqcls = ch_tok(']'),
        corkscrew = str_tok(":-"),
        space_tok = str_tok(" "),
        period = ch_tok('.'),
        comma = ch_tok(','),
        vertical = ch_tok('|'),
        moduleKeyword = str_tok("module"),
        typeKeyword = str_tok("type"),
        empty = str_tok("");

    auto notKeywordChecker = is_not((str("module") | str("type")) >> is_not(alphaNum | ch('_')));

    StrParser identifierStringParser = notKeywordChecker >>
                                       ((range('a', 'z') | ch('_')) +
                                        many(letter | digit | ch('_'))),

              variableStringParser = (range('A', 'Z') +
                                      many(alphaNum | ch('_')));

    // Global parsers
    PrologObjectParser identifier = token(call1(makeFromStringIdentifier, identifierStringParser)),
                       variable = token(call1(makeFromStringVariable, variableStringParser)),
                       program = undefined(),
                       expression = undefined(),
                       relationDeclaration = undefined(),
                       atom = undefined(),
                       primitive = undefined(),
                       moduleDeclaration = undefined(),
                       typeDeclaration = undefined();
    {
        // Primitive parser

        PrologObjectParser
            list = undefined(),
            listItems = undefined(),
            listHeadTail = undefined();

        setLazy(list, choice(
                          call1(makeList<obj>, sqopn >> listHeadTail >> sqcls),
                          call1(makeList<obj>, sqopn >> listItems >> sqcls)));

        setLazy(listHeadTail, call2(makeListHeadTail<obj, obj>, primitive >> vertical >> variable));
        setLazy(listItems, sepByStrict<AccumulatorListItems>(primitive, comma));
        setLazy(primitive, list | atom | variable);
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

        setLazy(expressionTerm, atom |
                                    opn >> expressionOperation >> cls |
                                    opn >> expressionTerm >> cls);

        setLazy(expressionOperation, _operators);
        setLazy(expression, expressionOperation | expressionTerm);
    }
    {
        // Declaration parser
        setLazy(relationDeclaration, choice(
                                         call2(makeRelation<obj, obj>, atom >> corkscrew >> expression >> period),
                                         call1(makeRelation<obj>, atom >> period)));
    }
    {
        // Atom parser
        PrologObjectParser
            primitiveInBrackets = undefined();

        setLazy(primitiveInBrackets, opn >> primitiveInBrackets >> cls | primitive);
        setLazy(atom,
                call2(makeAtom<obj, obj>, identifier >> sepBy<AccumulatorAtomSequence>(
                                                            primitiveInBrackets,
                                                            whitespace)));
    }
    {
        // Module parser
        setLazy(moduleDeclaration, call1(makeFromStringModuleDecl, moduleKeyword >> identifierStringParser >> period));
    }
    {
        // Type parser
        PrologObjectParser
            type = undefined(),
            _operators = undefined(),
            typeTerm = undefined(),
            typeOperation = undefined(),
            typeDeclarationHead = undefined();

        _operators = op_table(typeTerm)
                         ->infix_right("->", 10, makeArrow<obj, obj>);

        setLazy(typeTerm, atom | opn >> typeOperation >> cls | opn >> typeTerm >> cls);
        setLazy(typeOperation, _operators);
        setLazy(type, typeOperation | typeTerm);
        setLazy(typeDeclarationHead, call1(makeFromStringTypeDecl, typeKeyword >> identifierStringParser));
        setLazy(typeDeclaration, call2(makeTypeDecl<obj, obj>, typeDeclarationHead >> type >> period));
    }
    {
        // Program parsers

        PrologObjectParser
            relationDeclarationSequence = undefined(),
            typeDeclarationSequence = undefined();

        setLazy(relationDeclarationSequence, many1<AccumulatorRelationSequence>(relationDeclaration));
        setLazy(typeDeclarationSequence, many1<AccumulatorTypeDeclarationSequence>(typeDeclaration));

        setLazy(program,
                call3(makeProgram<obj, obj, obj>, moduleDeclaration >> typeDeclarationSequence >> relationDeclarationSequence) |
                    call2(makeProgram<obj, obj>, typeDeclarationSequence >> relationDeclarationSequence) |

                    call2(makeProgram<obj, obj>, moduleDeclaration >> typeDeclarationSequence) |
                    call1(makeProgram<obj>, typeDeclarationSequence) |

                    call2(makeProgram<obj, obj>, moduleDeclaration >> relationDeclarationSequence) |
                    call1(makeProgram<obj>, relationDeclarationSequence) |

                    call1(makeProgram<obj>, moduleDeclaration) |
                    call0(makeProgram<>, empty));
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
        for (; badStringBegin >= 1 && printer.text[badStringBegin - 1] != '\n'; --badStringBegin)
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
            os << (badCharPosition == i ? "^" : "_");
        }
        os << "\nError: "
           << "unexpected character '"
           << printer.text[printer.result.parse_position()]
           << "' at position "
           << badStringLineN + 1 << ':' << badCharPosition + 1
           << std::endl;
    }

    return os;
}
} // namespace prolog
