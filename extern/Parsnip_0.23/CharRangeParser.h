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


#ifndef PARSNIP_CHAR_RANGE_PARSER_H
#define PARSNIP_CHAR_RANGE_PARSER_H

#include <string>
using std::string;

#include "Parser.h"

namespace Parsnip
{
struct CharRangeParser : public IParser<string, string>
{
	CharRangeParser(char l, char u) : lower(l), upper(u) 
	{
		this->setName(string("CharRangeParser(`") + l + "' - `" + u + "')");
	}

protected:
	virtual Result<string> eval()
	{
		string val;
		
		if (Reader<string>::hasNext())
		{
			Reader<string>::IndexT lastPos = Reader<string>::pos();

			char currChar = Reader<string>::get();
			
			if (currChar >= lower && currChar <= upper)
			{
				val += currChar;
				return Result<string>::succeed(val);
			}
			
			//token-level parsers mustn't advance the cursor past where they succeed
			Reader<string>::set_pos(lastPos);
		}
		
		return Result<string>::fail();
	}
	
private:
	char lower, upper;	
};

ptr<IParser<string, string> > range(char l, char u)
{
	static Memoizer<std::pair<char, char>, ptr<IParser<string, string> > > memoizer;
	std::pair<char, char> c_pair = std::make_pair(l,u);
	if (memoizer.contains(c_pair)) return memoizer.get(c_pair);
	return memoizer.insert(c_pair, new  CharRangeParser(l, u));
}



}

#endif