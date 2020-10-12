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

#ifndef PARSNIP_H
#define PARSNIP_H

#include "ParseResult.h"
#include "AllParsers.h"

namespace Parsnip
{

template <typename In, typename Out>
ParseResult<Out> parse(const string& str, ptr<IParser<In, Out> > parser)
{
	Reader<In>::init_stream(str);

	Result<Out> result = parser->parse();
	CacheBase::clear_all();

	if (result) return ParseResult<Out>(result.data(), Reader<In>::pos(), str.length());

	return ParseResult<Out>(Reader<In>::pos(), str.length());
}

template <typename In, typename Out>
ParseResult<Out> parse_file(const string& name, ptr<IParser<In, Out> > parser)
{
	return parse(read_file(name), parser);
}

template <typename In, typename Out>
bool match(const string& str, ptr<IParser<In, Out> > parser)
{
	return parse(str, parser).valid;
}

}

#endif
