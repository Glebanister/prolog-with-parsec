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

#ifndef PARSNIP_MANY_PARSER_H
#define PARSNIP_MANY_PARSER_H

#include <limits>

#include "Parser.h"

namespace Parsnip
{

template <typename Accumulator,  typename AccOut, typename In, typename ParserOut>
struct ManyParser : public IParser<In, AccOut>
{
	ManyParser(ptr<IParser<In, ParserOut> > p, int _min, int _max) : parser(p), min(_min), max(_max) 
	{
		this->setName("many");
	}	
	
protected:
	virtual Result<AccOut> eval()
	{
		Accumulator acc;
		Result<ParserOut> result;
		
		//save position before trying to parse so we can restore it if we fail
		typename Reader<In>::IndexT lastPos = Reader<In>::pos();

		int i = 0;
		while (i < max && Reader<In>::hasNext()) {
			
			result = parser->parse();
			if (result) 
			{
				acc.accum( result.data());
			}
			else { break; }
			
			++i;
		} 
		
		if (i < min)
		{
			Reader<In>::set_pos(lastPos);	
			return Result<AccOut>::fail();
		}
		else
		{
			return Result<AccOut>::succeed(acc.result());			
		}
	}
	
	ptr<IParser<In, ParserOut> > parser; 
	int min, max;
};



template < typename Acc, typename In, typename Out>
ptr<IParser<In, typename Acc::ResultType> > many(ptr<IParser<In, Out> > a, int min = 0, int max = std::numeric_limits<int>::max())
{
	return new ManyParser<Acc, typename Acc::ResultType, In, Out>(a, min, max);
}

template <typename Acc, typename In, typename Out>
ptr<IParser<In, typename Acc::ResultType> > many1(ptr<IParser<In, Out> > a)
{
	return many<Acc>(a, 1);
}

template <typename Acc, typename In, typename Out>
ptr<IParser<In, typename Acc::ResultType> > atleast(ptr<IParser<In, Out> > a, int min)
{
	return many<Acc>(a, min);
}



/*
template <typename In, typename Out>
ptr<IParser<std::map<Out, Out>, In> > manyMap(ptr<IParser<Vector<Out, 2> > > vecParser, int min = 0, int max = INT_MAX)
{
	return new ManyParser< BuildMapFromVec<Out> , std::map<Out, Out>, In, Vector<Out, 2>>(vecParser, min, max);
}

template <typename In, typename Out>
ptr<IParser<std::map<Out, Out>, In> > manyMap1(ptr<In, IParser<Vector<Out, 2> > > vecParser)
{
	return manyMap(vecParser, 1);
}

template <typename In, typename Out>
ptr<IParser<std::map<Out, Out>, In> > atleastMap(ptr<IParser<In, Vector<Out, 2> > > vecParser, int min)
{
	return manyMap(vecParser, min);
}
*/

}
#endif