#include <iostream>
#include <string>

#include "ArgumentParser.hpp"
#include "Parsnip.h"

using namespace Parsnip;

struct PrologObject
{
    PrologObject(std::string code = "", std::string type = "")
        : code(std::move(code)), type(std::move(type)) {}

    std::string code;
    std::string type;
    std::vector<ptr<PrologObject>> objects;
};

std::ostream &operator<<(std::ostream &os, const PrologObject &object)
{
    std::string code = "";
    if (!object.code.empty())
    {
        code = "'" + object.code + "'";
    }
    os << object.type << ' ' << code;
    if (object.objects.empty())
    {
        os << ' ';
        return os;
    }
    os << "(";
    std::for_each(object.objects.begin(), object.objects.end(), [&](const auto &obj) { os << *obj; });
    os << ") ";
    return os;
}

using PrologObjectPtr = ptr<PrologObject>;

struct AccumulatePrologObject : public Accumulator<PrologObjectPtr, PrologObjectPtr>
{
    AccumulatePrologObject(std::string code, std::string type)
    {
        object = new PrologObject(code);
        object->code = std::move(code);
        object->type = std::move(type);
    }

    void accum(const PrologObjectPtr &obj)
    {
        object->objects.push_back(obj);
    }

    PrologObjectPtr result()
    {
        return object;
    }

private:
    PrologObjectPtr object;
};

PrologObjectPtr makeIdentifier(std::string name)
{
    return new PrologObject(name, "Identifier");
}

#define OBJECT_MAKER(name)                      \
    template <typename... Args>                 \
    PrologObjectPtr make##name(Args... args)    \
    {                                           \
        auto obj = new PrologObject("", #name); \
        (obj->objects.push_back(args), ...);    \
        return obj;                             \
    }

#define OBJECT_FROM_STRING_MAKER(name)                  \
    PrologObjectPtr makeFromString##name(std::string s) \
    {                                                   \
        auto obj = new PrologObject(s, #name);          \
        return obj;                                     \
    }

OBJECT_FROM_STRING_MAKER(Identifier);
OBJECT_MAKER(Disj);
OBJECT_MAKER(Conj);
OBJECT_MAKER(Decl);
OBJECT_MAKER(Atom);
OBJECT_MAKER(AtomSeq);

int main(int argc, const char **argv)
{
    using std::string;
    {

        using PrologObjectParser = yasper::ptr<Parsnip::IParser<std::string, PrologObjectPtr>>;
        using obj = PrologObjectPtr;
        auto undefined = []() { return lazy<std::string, PrologObjectPtr>(); };
        auto ch_tok = [](char c) { return token(skip_ch(c)); };
        auto str_tok = [](std::string s) { return token(skip_str(std::move(s))); };

        // Tokens
        PrologObjectParser identifier = token(call1(makeFromStringIdentifier, (letter | ch('_')) + optional(many1(letter | digit | ch('_')))));
        auto opn = ch_tok('('),
             cls = ch_tok(')'),
             corkscrew = str_tok(":-"),
             period = ch_tok('.');

        // Global parsers
        PrologObjectParser
            expression = undefined(),
            declaration = undefined(),
            atom = undefined();

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
            setLazy(declaration, call2(makeDecl<obj, obj>, atom >> corkscrew >> expression >> period) |
                                     call1(makeDecl<obj>, atom >> period));
        }
        {
            // Atom parsers
            PrologObjectParser
                atomSeq = undefined(),
                atomBrackets = undefined();

            setLazy(atom, call2(makeAtom<obj, obj>, identifier >> atomSeq));
            setLazy(atomSeq, call2(makeAtomSeq<obj, obj>, opn >> atomBrackets >> cls >> atomSeq) |
                                 optional<string, obj>(atom, new PrologObject("", "Atom")));
            setLazy(atomBrackets, opn >> atomBrackets >> cls | atom);
        }

        std::vector<string>
            tests = {
                "a:-b.",
                "a.",
                "a :- x.",
                "a :- (x).",
                "a :- x, y, z.",
                "a :- x, y; z.",
                "a :- x, (y; z).",
                "other_name :- x, ((y; z)).",
                "other_name :- x, (y; (a, b, c, d, e; f) ,z).",
                "x :- x, (y; (a, b, c, d, (e); f) ,z).",
                "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
                "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
                "x :- x, (y; (a, b, c d, (e); f) ,z).",
                "a :- a a a a a.",
                "a :- a a a a a, a a a; a a a, a.",
                "a (((a))) :- (((a))), a.",
                "a (a) a ((a)) :- a, a, a (a).",
                "a :- (a).",
                "a :- (a), (b), (c).",
            };
        for (auto test : tests)
        {
            auto result = parse(test, declaration);
            if (result.parse_finished())
            {
                std::cout << "Success: " << *result.data() << std::endl;
                continue;
            }
            std::cout << test << ' ';
            
            if (result.input_consumed())
            {
                std::cout << "Error: "
                          << "unexpected end of input" << std::endl;
            }
            else
            {
                std::cout << "Error: "
                          << "unexpected character '" << test[result.parse_position()] << "' at position " << result.parse_position() + 1 << endl;
            }
        }
    }

    // using NumParser = Parser<string, double>::type;

    // // prolog::PrologCheckerArgumentParser args(argc, argv);

    // NumParser exprOper = lazy<string, double>();
    // NumParser term = lazy<string, double>();
    // setLazy(term, real | skip_ch('(') >> exprOper >> skip_ch(')') | skip_ch('(') >> term >> skip_ch(')'));
    // NumParser ops = op_table(term)
    //                     ->infix_left("+", 10, [](double a, double b) { return a + b; })
    //                     ->infix_left("*", 20, [](double a, double b) { return a * b; });

    // setLazy(exprOper, ops);

    // NumParser expr = exprOper | term;

    // std::string input;
    // ParseResult<double> result;

    // while (true)
    // {
    //     std::cout << "> ";
    //     std::getline(std::cin, input);

    //     if (input == "exit")
    //         break;

    //     result = parse(input, expr);

    //     if (result.parse_finished())
    //     {
    //         cout << endl
    //              << result.data() << endl;
    //     }
    //     else if (result.input_consumed())
    //     {
    //         cout << "Unexpected end of input string " << endl;
    //     }
    //     else
    //     {
    //         std::string::size_type pos = result.parse_position();
    //         cout << "Unexpected character '" << input[pos] << "' at position " << pos << endl;
    //     }
    // }

    // return 0;
}
