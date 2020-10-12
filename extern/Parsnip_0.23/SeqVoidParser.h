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

#ifndef PARSNIP_SEQ_VOID_PARSER
#define PARSNIP_SEQ_VOID_PARSER

#include "Parser.h"

namespace Parsnip
{

/* 
	sequence when both parsers return void 
*/

template <typename In>
struct SeqVoidParser : public IParser<In, void>
{
	SeqVoidParser( ptr<IParser<In, void> > a, ptr<IParser<In, void> > b ) : first(a), second(b) {}
	
	virtual Result<void> eval()
	{
		Result<void> result1 = first->parse();
		if(result1)
		{ 
			Result<void> result2 = second->parse();
		
			if (result2) 
			{
				return Result<void>::succeed();
			}
			
			return Result<void>::fail();
		}
		return Result<void>::fail();
	
	}
	
	ptr<IParser<In, void> > first;
	ptr<IParser<In, void> > second;
};

}

#endif