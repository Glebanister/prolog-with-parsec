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

#ifndef PARSNIP_STRING_PARSER_H
#define PARSNIP_STRING_PARSER_H

	
#include <string>
using std::string;

#include "Parser.h"

namespace Parsnip
{

struct StringParser : public IParser<string, string>
{
	StringParser(const std::string& _str) : str(_str) 
	{
		this->setName(string("StringParser(\"") + str + "\")");
	}

protected:
	virtual Result<string> eval()
	{
		unsigned i = 0;
		char currChar;
		while (i < str.length() && Reader<string>::hasNext())
		{
			currChar = Reader<string>::get();
			if (currChar != str[i]) 
			{	
				Reader<string>::rewind();
				break;	
			}
			++i;
		}

		//if string was exhausted, we succeeded
		if (i == str.length())
		{
			return Result<string>::succeed(str);
		}
		
		return Result<string>::fail();
	}
	
private:
	string str;
};



ptr<IParser<string, string> >  str(const string& s)
{	
	static Memoizer<string, ptr<IParser<string, string> > > memoizer;
	if (memoizer.contains(s)) return memoizer.get(s);
	return memoizer.insert(s, new StringParser(s));
}

}

#endif