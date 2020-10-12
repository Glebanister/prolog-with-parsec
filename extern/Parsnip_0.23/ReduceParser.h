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

#ifndef REDUCE_PARSER_H
#define REDUCE_PARSER_H

#include <memory>

#include "yasper.h"

namespace Parsnip
{
	template <typename In, typename Out, unsigned int N>
	struct ReduceParser : public IParser<In, Vector<Out, N>>
	{
		ReduceParser(ptr<IParser<In, Vector<Out, N>>> _parser, Out (*fn)(Out, Out))
			: parser(_parser), reduceFn(fn) {}

		virtual Result<Out> eval()
		{
			Result<Vector<Out, N>> result = parser->parse();
			if (result)
			{
				Vector<Out, N> vec = result.get();

				Out value = vec[0];
				for (unsigned i = 1; i < N; ++i)
				{
					value = reduceFn(value, vec[i]);
				}

				return Result<Out>::succeed(value);
			}
			else
			{
				return Result<Out>::fail();
			}
		}

		ptr<IParser<In, Vector<Out, N>>> parser;
		Out (*reduceFn)(Out, Out);
	};

	template <typename In, typename Out, unsigned int N>
	yasper::ptr<IParser<In, Out>> reduce(Out (*reduceFn)(Out, Out),
										 ptr<IParser<In, Vector<Out, N>>> parser);
	// {
	// 	// return std::make_unique<ReduceParser>(parser, reduceFn).release();
	// }

} // namespace Parsnip

#endif
