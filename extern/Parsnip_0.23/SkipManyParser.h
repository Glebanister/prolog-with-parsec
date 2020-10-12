
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
#ifndef PARSNIP_SKIP_MANY_PARSER_H
#define PARSNIP_SKIP_MANY_PARSER_H

#include "Parser.h"

namespace Parsnip
{

template <typename In, typename Out>
struct SkipManyParser : public IParser<In, void>
{
	SkipManyParser(ptr<IParser<In, Out> > p, int _min, int _max) : parser(p), min(_min), max(_max) 
	{
		this->setName("skipMany");
	}	
	
protected:
	virtual Result<void> eval()
	{	
		Result<Out> result;
		
		//save position before trying to parse so we can restore it if we fail
		typename Reader<In>::IndexT lastPos = Reader<In>::pos();

		int i = 0;
		while (i < max && Reader<In>::hasNext()) {
			
			result = parser->parse();
			if (!result) 
			{
				break; 
			}
			
			++i;
		} 
		
		if (i < min)
		{
			Reader<In>::set_pos(lastPos);	
			return Result<void>::fail();
		}
		else
		{
			return Result<void>::succeed();			
		}
	}
	
	ptr<IParser<In, Out> > parser; 
	int min, max;
};


template <typename In, typename Out>
ptr<IParser<In, void> > skipMany(ptr<IParser<In, Out> > a, int min = 0, int max = std::numeric_limits<int>::max())
{
	return new SkipManyParser<In, Out>(a, min, max);
}


template <typename In, typename Out>
ptr<IParser<In, void> > skipMany1(ptr<IParser<In, Out> > a)
{
	return skipMany(a, 1);
}


template <typename In, typename Out>
ptr<IParser<In, void> > skipAtleast(ptr<IParser<In, Out> > a, int min)
{
	return skipMany(a, min);
}


}

#endif