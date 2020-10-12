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

#ifndef PARSNIP_SEQ_USE_PARSER
#define PARSNIP_SEQ_USE_PARSER

#include "Parser.h"

namespace Parsnip
{

/* 
	sequence when first parser is void 
*/

template <typename  In, typename Out>
struct SeqUseSecondParser : public IParser<In, Out>
{
	SeqUseSecondParser( ptr<IParser<In, void> > a, ptr<IParser<In, Out> > b ) : first(a), second(b) 
	{
		this->setName("seq");
	}

protected:
	virtual Result<Out> eval()
	{
		Result<void> result1 = first->parse();

		if(result1)
		{ 
			Result<Out> result2 = second->parse();
		
			if (result2) 
			{
				return Result<Out>::succeed(result2.data());
			}

			return Result<Out>::fail();
		}

		return Result<Out>::fail();
	}
	
	ptr<IParser<In, void> > first;
	ptr<IParser<In, Out> > second;
};


/* 
	sequence when second parser is void 
*/

template <typename  In, typename Out>
struct SeqUseFirstParser : public IParser<In,  Out>
{
	SeqUseFirstParser( ptr<IParser<In, Out> > a, ptr<IParser<In, void> > b ) : first(a), second(b) 
	{
		this->setName("seq");
	}
	
	virtual Result<Out> eval()
	{
		Result<Out> result1 = first->parse();

		if(result1)
		{ 
			Result<void> result2 = second->parse();
		
			if (result2) 
			{
				return Result<Out>::succeed(result1.data());
			}
			return Result<Out>::fail();
		}
		return Result<Out>::fail();

	}
	
	ptr<IParser<In, Out> > first;
	ptr<IParser<In, void> > second;
};

}

#endif