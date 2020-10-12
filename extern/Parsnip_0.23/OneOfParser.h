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

#ifndef PARSNIP_ONE_OF_PARSER
#define PARSNIP_ONE_OF_PARSER

#include <string>
using std::string;

#include "Parser.h"

namespace Parsnip
{

struct OneOfParser : public IParser<string, string>
{
	OneOfParser(const std::string& _str) : str(_str) {}

protected:
	virtual Result<string> eval()
	{
		if (Reader<string>::hasNext())
		{
			Reader<string>::IndexT lastPos = Reader<string>::pos();
			
			char currChar = Reader<string>::get();
			
			for (string::size_type i = 0; i < str.length(); ++i)
			{
				std::string val;
				if (str[i] == currChar)
				{
					val += currChar;
					return Result<string>::succeed(val);
				}
			}
			
			//token-level parsers mustn't advance the cursor past where they succeed
			Reader<string>::set_pos(lastPos);
		}
		
		return Result<string>::fail();
	}
	
private:
	string str;

	OneOfParser();
};

ptr<IParser<string, string> > oneOf(const std::string& str)
{
	return new OneOfParser(str);
}

}

#endif