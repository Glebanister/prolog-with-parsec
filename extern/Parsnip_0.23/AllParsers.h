/*
    Parsnip Parser Libary
	http://parsnip-parser.sf.net
	Copyright 2007 Alex Rubinsteyn
	-----------------------------------------------------------------------
	This file is part of Parsnip.

    Parsnip is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Parsnip is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Parsnip.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PARSNIP_ALL_PARSERS_H
#define PARSNIP_ALL_PARSERS_H

#include "FailParser.h"
#include "SucceedParser.h"
#include "SkipParser.h"
#include "SeqOperators.h"
#include "SeqVecOperators.h"
#include "ChoiceParser.h"
#include "LazyParser.h"
#include "OptionalParser.h"
#include "ManyParser.h"
#include "SkipManyParser.h"
#include "SepByParser.h"
#include "LongerParser.h"
#include "CreateParser.h"
#include "CallParser.h"
#include "CharParser.h"
#include "CharCombinators.h"
#include "StringParser.h"
#include "CharRangeParser.h"
#include "OneOfParser.h"
#include "StringConcatParser.h"
#include "StringCombinators.h"
#include "NotParser.h"
#include "ReduceParser.h"
#include "Accumulators.h"
#include "ConversionFunctions.h"
#include "PrettyPrinters.h"
#include "Whitespace.h"
#include "OpTable.h"

namespace Parsnip
{

	template <typename Out>
	ptr<IParser<std::string, Out>> token(ptr<IParser<std::string, Out>> parser)
	{
		return skip(whitespace) >> parser >> skip(whitespace);
	}

	template <typename Out>
	ptr<IParser<std::string, void>> skip_token(ptr<IParser<std::string, Out>> parser)
	{
		return token(skip(parser));
	}

	Parser<string, string>::type token_range(char l, char u)
	{
		return token(range(l, u));
	}

	Parser<string, string>::type token_ch(char c)
	{
		return token(ch(c));
	}

	Parser<string, string>::type token_str(const string &s)
	{
		return token(str(s));
	}

	ptr<IParser<std::string, void>> skip_range(char l, char u)
	{
		return skip(range(l, u));
	}

	ptr<IParser<std::string, void>> skip_ch(char c)
	{
		return skip(ch(c));
	}
	ptr<IParser<std::string, void>> skip_str(const string &s)
	{
		return skip(str(s));
	}

	ptr<IParser<std::string, void>> skip_token_range(char l, char u)
	{
		return skip(token(range(l, u)));
	}

	ptr<IParser<std::string, void>> skip_token_ch(char c)
	{
		return skip(token(ch(c)));
	}

	ptr<IParser<std::string, void>> skip_token_str(const string &s)
	{
		return skip(token(str(s)));
	}

	template <typename In, typename Out>
	ptr<IParser<In, void>> skip_optional(ptr<IParser<In, Out>> p)
	{
		return skip(optional(p));
	}

	ptr<IParser<std::string, void>> not_ch(char c)
	{
		return is_not(ch(c));
	}

	ptr<IParser<std::string, void>> not_str(const string &s)
	{
		return is_not(str(s));
	}

	ptr<IParser<std::string, void>> not_range(char l, char u)
	{
		return is_not(range(l, u));
	}

	template <unsigned int N>
	Parser<string, string>::type flatten(ptr<IParser<Vector<std::string, N>, std::string>> parser)
	{
		return reduce(concat<std::string>, parser);
	}

	typedef const Parser<std::string, std::string>::type StrParser;

	const StrParser anyChar = range(1, 127) >>= "anyChar";

	const StrParser letter = range('a', 'z') | range('A', 'Z') >>= "letter";

	const StrParser punctuation = ch('!') | ch('?') | ch('.') | ch(',') | ch('\'') | ch('\"') | ch(';') | ch(':') | ch('&') | ch('(') | ch(')') | ch('[') | ch(']') >>= "punctuation";

	const StrParser single_quote = ch('\'') >>= "single_quote";
	const StrParser double_quote = ch('"') >>= "double_quote";

	const StrParser digit = range('0', '9') >>= "digit";

	const StrParser bit = ch('0') | ch('1') >>= "bit";

	const StrParser hexDigit = range('0', '9') | range('a', 'f') | range('A', 'F') >>= "hexDigit";

	const StrParser octDigit = range('0', '7') >>= "octDigit";

	const StrParser alphaNum = letter | digit >>= "alphaNum";

	/* 
		Word parser is any letter of the alphabet 
	 */
	const StrParser letters = many1(letter) >>= "letters";

	/*
	 numerical parsers
	 */
	const StrParser digits = many1(digit) >>= "digits";

	const StrParser binDigits = many1(bit) >>= "binDigits";

	const StrParser hexDigits = many1(hexDigit) >>= "hexDigits";

	const StrParser octDigits = many1(octDigit) >>= "octDigits";

	const StrParser int_str = optional(ch('-')) + digits >>= "int_str";

	const StrParser real_str = int_str + optional(ch('.') + digits) >>= "real_str";

	const StrParser complex_str = real_str + token(ch('+') | ch('-')) + real_str + ch('i') | ch('j') >>= "complex_str";

	const StrParser num_str = real_str | complex_str >>= "num_str";

	const Parser<string, unsigned>::type natural = call1(fromString<unsigned>, digits) >>= "natural";

	const Parser<string, int>::type integer = call1(fromString<int>, int_str) >>= "integer";
	const Parser<string, double>::type real = call1(fromString<double>, real_str) >>= "real";

	const StrParser JavaComment = between(str("/*"), many(not_str("*/") >> anyChar), str("*/"));

} // namespace Parsnip

#endif