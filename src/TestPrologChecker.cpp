#include "PrologChecker.hpp"

int main()
{
    enum testSuiteType
    {
        ALL_CORRECT,
        ALL_INCORRECT,
    };
    auto testSuite = [](const std::string &testSuitName, const std::vector<std::string> &tests, testSuiteType type) {
        for (const auto &test : tests)
        {
            auto result = prolog::parseProgram(test);
            if (result.parse_finished() && type == ALL_INCORRECT)
            {
                std::cout << "Test suite: " << testSuitName << ", Test:\n"
                          << test << std::endl
                          << "Is not correct, but no errors were found\n"
                          << prolog::ParsingResultPrinter(result, test)
                          << std::endl;
            }
            else if (!result.parse_finished() && type == ALL_CORRECT)
            {
                std::cout << "Test suite: " << testSuitName << ", Test:\n"
                          << test << std::endl
                          << "Is correct, but error was found:\n"
                          << prolog::ParsingResultPrinter(result, test)
                          << std::endl;
            }
        }
    };

    testSuite("Correct relation declarations",
              {
                  "",
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
              },
              ALL_CORRECT);

    testSuite("Incorrect relation declarations",
              {
                  "1",
                  ".",
                  ":-",
                  "a :-",
                  "a:-1.",
                  "a:-.",
                  ":-.",
                  "a :- (x, y, z.",
                  "a :- x, y))) z.",
                  "(a :- x, ,y; z.)",
                  "(a :- x, ,y; z.).",
                  "a :- 1.",
                  "a :- x..",
                  "1badname :- x, (y; z).",
                  "other_name :- x, (y; ((a, b, c, d, e; f) ,z).",
                  "x :- x, (y; a, b, c, d, (e); f) ,z), a ; d.",
                  "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; .",
                  "a :- (a) (a) a a a.",
                  "a (((a)) :- (((a))), a.",
                  "a (a) a ((a)) :- a, a, a ((a) a (a)).",
                  "a :- ().",
                  "(a) :- a.",
                  "a () :- (a), (b), (c).",
                  "f : - f.",
              },
              ALL_INCORRECT);

    testSuite("Correct type declarations",
              {
                  "type typeName a -> b -> c.",
                  "type typeName a b.",
                  "type typeName a.",
                  "type moduleAsPrefixName a.",
                  "type _typename (a -> a) -> (b) -> (c -> c).",
                  "type _typename123 (x -> (x -> x -> (x))).",
                  "type name (a b) -> (c d).",
                  "   type    name    ( ( a c (c) -> x) -> y).   ",
              },
              ALL_CORRECT);

    testSuite("Incorrect type declarations",
              {
                  "type typeName module -> b -> c.",
                  "tupe typeName a -> b -> c.",
                  "type typeName a",
                  "type ",
                  "type a",
                  "type a x ->",
                  "type typeName a module.",
                  "module typeName a b.",
                  "type Typename (a -> a) -> (b) -> (c -> c).",
                  "type _typename123 (x -> (x -> x -> x))).",
                  "type _typename123 (x -> (NotIdentificator -> x -> x))).",
                  "type _typename123 (x -> (x -> x -> x))).",
                  "type (a b) - (c d).",
                  "   Type    name    ( ( a c (c) -> x) -> y).   ",
              },
              ALL_INCORRECT);
}
