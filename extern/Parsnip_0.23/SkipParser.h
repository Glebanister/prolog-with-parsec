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

#ifndef PARSNIP_SKIP_PARSER_H
#define PARSNIP_SKIP_PARSER_H

#include "Parser.h"

namespace Parsnip
{

	/*
	Skipping a parse means have a parser of type <In, void>
	which consumes input but returns nothing
*/

	template <typename In, typename Out>
	struct SkipParser : public IParser<In, void>
	{
		SkipParser(ptr<IParser<In, Out>> p) : myParser(p) {}

		Result<void> eval()
		{
			Result<Out> result = myParser->parse();
			return result ? Result<void>::succeed() : Result<void>::fail();
		}
		ptr<IParser<In, Out>> myParser;
	};

	template <typename In, typename Out>
	ptr<IParser<In, void>> skip(const ptr<IParser<In, Out>> &parser)
	{
		ptr<IParser<In, void>> p = new SkipParser<In, Out>(parser);
		p->setName("skip {" + p->getName() + "}");
		return p;
	}

	template <typename In, typename Out>
	ptr<IParser<In, Out>> between(ptr<IParser<In, Out>> left_delim, ptr<IParser<In, Out>> center, ptr<IParser<In, Out>> right_delim)
	{
		return skip(left_delim) >> center >> skip(right_delim);
	}

} // namespace Parsnip

#endif