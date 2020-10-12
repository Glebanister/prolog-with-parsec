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

#ifndef PARSNIP_STRING_CONCAT_PARSER_H
#define PARSNIP_STRING_CONCAT_PARSER_H

#include <string>
using std::string;

#include "Parser.h"
#include "ManyParser.h"

namespace Parsnip
{
	
struct StringConcatParser : public IParser<string, string>
{

	StringConcatParser(ptr<IParser<string, string> > _a, ptr<IParser<string, string> > _b) : a(_a), b(_b) 
	{
		this->setName("str_concat");
	}

protected:
	virtual Result<string> eval()
	{
		Result<string> result1 = a->parse();

		if (result1)
		{
			Result<string> result2 = b->parse();
			if (result2) 
			{
				return Result<string>::succeed(result1.data() + result2.data());
			}
			
			return Result<string>::fail();
		}
		
		return Result<string>::fail();	
	}

	ptr<IParser<string, string> > a;
	ptr<IParser<string, string> > b;
};


ptr<IParser<string, string> > concat (ptr<IParser<string, string> > a, ptr<IParser<string, string> > b)
{
	typedef IParser<string, string> StrParser;
	static Memoizer<std::pair<StrParser*, StrParser*>, ptr<StrParser> > memoizer;
	std::pair<StrParser*, StrParser*> p_pair = std::make_pair(a.GetRawPointer(), b.GetRawPointer());
	if (memoizer.contains(p_pair)) return memoizer.get(p_pair);
	return memoizer.insert(p_pair, new StringConcatParser(a,b));
}



ptr<IParser<string, string> > operator+ (ptr<IParser<string, string> > a, ptr<IParser<string, string> > b)
{
		return concat(a, b);
}

ptr<IParser<string, string> > operator+ (ptr<IParser<string, void> > a, ptr<IParser<string, string> > b)
{
	return a >> b;
}


ptr<IParser<string, string> > operator+ (ptr<IParser<string, string> > a, ptr<IParser<string, void> > b)
{
	return a >> b;
}

ptr<IParser<string, void> > operator+ (ptr<IParser<string, void> > a, ptr<IParser<string, void> > b)
{
	return a >> b;
}

}

#endif