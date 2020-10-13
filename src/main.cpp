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

#define OBJECT_FROM_VOID_MAKER(name)            \
    PrologObjectPtr makeFromVoid##name()        \
    {                                           \
        auto obj = new PrologObject("", #name); \
        return obj;                             \
    }

OBJECT_FROM_STRING_MAKER(Identifier);
OBJECT_MAKER(Decl);
OBJECT_MAKER(DeclWithoutPeriod);
OBJECT_FROM_STRING_MAKER(Corckscrew);
OBJECT_FROM_STRING_MAKER(Period);
OBJECT_MAKER(Body);
OBJECT_MAKER(Disj);
OBJECT_MAKER(Conj);
OBJECT_MAKER(OpOr);

int main(int argc, const char **argv)
{
    using std::string;
    {
        using PrologObjectParser = yasper::ptr<Parsnip::IParser<std::string, PrologObjectPtr>>;
        using obj = PrologObjectPtr;
        using parser = PrologObjectParser;
        auto UndefinedObj = []() {
            return lazy<std::string, PrologObjectPtr>();
        };

        parser identifier = call1(makeFromStringIdentifier, (letter | ch('_')) + optional(many1(letter | digit | ch('_'))));
        parser decl = UndefinedObj(),
               body = UndefinedObj(),
               declWithoutPeriod = UndefinedObj(),
               term = UndefinedObj(),
               expr = UndefinedObj(),
               opSelf = UndefinedObj(),
               operators = UndefinedObj();

        parser corckscrew = call1(makeFromStringCorckscrew, token_str(":-"));
        parser period = call1(makeFromStringPeriod, token_str("."));

        setLazy(decl, call2(makeDecl<obj, obj>, declWithoutPeriod >> period) |
                          call2(makeDecl<obj, obj>, identifier >> period));
        setLazy(declWithoutPeriod, call3(makeDeclWithoutPeriod<obj, obj, obj>, identifier >> corckscrew >> body));
        setLazy(body, expr);

        setLazy(term, identifier | skip_ch('(') >> opSelf >> skip_ch(')'));
        operators = op_table(term)
                        ->infix_right(";", 10, makeDisj<obj, obj>)
                        ->infix_right(",", 20, makeConj<obj, obj>);
        setLazy(opSelf, operators);
        setLazy(expr, operators | term);

        // PrologObjectParser atom = identifier >> atom_seq;

        // // StrParser atom_seq = lazy<string, string>();
        // // StrParser atom_brackets = lazy<string, string>();
        // // StrParser atom = identifier >> atom_seq;
        // // // setLazy(atom_seq, optional(atom) | (ch('(') >> atom_brackets >> ch(')')));
        // Parser<string, Tuple2<string, string>>::type single_pair = letters >> skip(token_ch('=')) >> letters >> skip_ch(';');
        // Parser<string, std::map<string, string>>::type pair_parser = many<BuildMap<string, string>>(token(single_pair));
        std::vector<string>
            tests = {
                // "a:-b.",
                // "a.",
                // "a :- x.",
                // "a :- (x).",
                // "a :- x, y, z.",
                // "a :- x, y; z.",
                // "a :- x, (y; z).",
                // "other_name :- x, ((y; z)).",
                // "other_name :- x, (y; (a, b, c, d, e; f) ,z).",
                // "x :- x, (y; (a, b, c, d, (e); f) ,z).",
                // "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
                // "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
                // "x :- x, (y; (a, b, c d, (e); f) ,z).",
                "a",
                "a, b",
                "a; b",
                "a   ; b",
                "a, b ; d, d",
            };
        for (auto test : tests)
        {
            auto result = parse(test, expr);
            if (result.parse_finished())
            {
                std::cout << "Success: " << *result.data() << std::endl;
            }
            else if (result.input_consumed())
            {
                std::cout << "Error: "
                          << "unexpected end of input" << std::endl;
            }
            else
            {
                std::cout << "Error: "
                          << "unexpected character '" << test[result.parse_position()] << "' at position " << result.parse_position() << endl;
            }
        }
    }

    // using NumParser = Parser<string, double>::type;

    // prolog::PrologCheckerArgumentParser args(argc, argv);

    // NumParser op_self = lazy<string, double>();
    // NumParser term = real | skip_ch('(') >> op_self >> skip_ch(')');
    // NumParser ops = op_table(term)
    //                     ->infix_left("+", 10, add)
    //                     ->infix_left("-", 10, subtract)
    //                     ->infix_left("*", 20, multiply)
    //                     ->infix_left("/", 20, divide);

    // setLazy(op_self, ops);

    // NumParser expr = ops | term;

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
