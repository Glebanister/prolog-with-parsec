#include <iostream>
#include <string>

#include "PrologChecker.hpp"

int main(int argc, const char **argv)
{
    using std::string;
    using namespace prolog;
    using namespace Parsnip;
    {
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
            expression = undefined(),
            relationDeclaration = undefined(),
            atom = undefined(),
            moduleDeclaration = undefined();

        {
            // Expression parsers
            // PrologObjectParser
            //     expressionTerm = undefined(),
            //     expressionOperation = undefined(),
            //     _operators = undefined();

            // _operators = op_table(expressionTerm)
            //                  ->infix_right(";", 10, makeDisj<obj, obj>)
            //                  ->infix_right(",", 20, makeConj<obj, obj>);

            // setLazy(expressionTerm, atom | opn >> expressionOperation >> cls | opn >> expressionTerm >> cls);
            // setLazy(expressionOperation, _operators);
            // setLazy(expression, expressionOperation | expressionTerm);
        }
        {
            // Declaration parser
            // setLazy(relationDeclaration,
            //         call1(makeDecl<obj>, atom >> period) |
            //             call2(makeDecl<obj, obj>, atom >> corkscrew >> expression >> period));
        }
        {
            // Atom parsers
            // PrologObjectParser
            //     atomSeq = undefined(),
            //     atomBrackets = undefined();

            // setLazy(atom, call2(makeAtom<obj, obj>, identifier >> atomSeq));
            // setLazy(atomSeq, call2(makeAtomSeq<obj, obj>, opn >> atomBrackets >> cls >> atomSeq) |
            //                      optional<string, obj>(atom, makeAtom<>()));
            // setLazy(atomBrackets, opn >> atomBrackets >> cls | atom);
        }
        {
            // Module parser
            // setLazy(moduleDeclaration, call1(makeFromStringModuleDecl, module >> identifier >> period));
        }

        {
            auto test = [&](const std::string &testSuite,
                            const std::vector<std::string> &tests,
                            PrologObjectParser parser,
                            bool areCorrect) {
                for (auto test : tests)
                {
                    auto result = parse(test, relationDeclaration);
                    if (result.parse_finished() && !areCorrect)
                    {
                        std::cout << test << '\n'
                                  << "is not correct, but has been recognized as " << testSuite << std::endl
                                  << PrologObjectPrinter(0, "| ", "- ", result.data()) << std::endl;
                        continue;
                    }
                    else if (result.parse_position() < test.size() - 1 && areCorrect)
                    {
                        std::cout << test << '\n'
                                  << "is correct but can not be parsed as " << testSuite;
                    }
                }
            };
            std::vector<string> testRelationDeclaration = {
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
                "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d",
                "x :- x, (y; (a, b, c, d, e); f) ,z), a ; d.",
                "x :- x, (y; (a, b, c d, (e); f) ,z).",
                "a :- a a a a a.",
                "a :- a a a a a, a a a; a a a, a.",
                "a (((a))) :- (((a))), a.",
                "a (a) a ((a)) :- a, a, a (a).",
                "a :- (a).",
                "a :- (a), (b), (c).",
            };
        }
    }
}
