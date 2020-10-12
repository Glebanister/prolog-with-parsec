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

#ifndef PARSNIP_STRING_PARSER_BASE_H
#define PARSNIP_STRING_PARSER_BASE_H

#include "StringConcatParser.h"
#include "OptionalParser.h"

namespace Parsnip
{

	/********************************************
	Redefine some combinators for strings
 ********************************************/

	/*
	sepBy should build lists of strings
*/

	ptr<IParser<string, std::list<string>>> sepBy(ptr<IParser<string, string>> data, ptr<IParser<string, string>> sep, int min = 0, int max = std::numeric_limits<int>::max())
	{
		return sepBy(data, sep, min, max);
	}

	ptr<IParser<string, std::list<string>>> sepBy1(ptr<IParser<string, string>> data, ptr<IParser<string, string>> sep, int max = std::numeric_limits<int>::max())
	{
		return sepBy(data, sep, 1, max);
	}

	ptr<IParser<string, std::list<string>>> sepByAtleast(ptr<IParser<string, string>> data, ptr<IParser<string, string>> sep, int min)
	{
		return sepBy(data, sep, min);
	}

	//optional string parsers return the empty string
	ptr<IParser<string, string>> optional(ptr<IParser<string, string>> parser)
	{
		return optional(parser, std::string(""));
	}

	/*
	many should concatenate strings by default 
*/

	ptr<IParser<string, string>> many(ptr<IParser<string, string>> a, int min = 0, int max = std::numeric_limits<int>::max())
	{
		return many<Concat<string>, string, string>(a, min, max);
	}

	ptr<IParser<string, string>> many1(ptr<IParser<string, string>> a)
	{
		return many(a, 1);
	}

	ptr<IParser<string, string>> atleast(ptr<IParser<string, string>> a, int min)
	{
		return many(a, min);
	}

} // namespace Parsnip

#endif