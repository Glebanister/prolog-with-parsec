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

#ifndef PARSNIP_WHITESPACE_H
#define PARSNIP_WHITESPACE_H

#include "CharParser.h"
#include "ManyParser.h"
#include "ChoiceParser.h"
#include "SkipParser.h"
namespace Parsnip
{	
	
	const Parser<string, string>::type space = ch(' ');
	const Parser<string, string>::type tab = ch('\t');
	const Parser<string, string>::type newline = ch('\n');


	const Parser<string, string>::type spaces = many(space);
	const Parser<string, string>::type tabs = many(tab);
	const Parser<string, string>::type newlines = many(newline);

	/*
		whitespace is mutable!
	*/
	Parser<string, string>::type whitespace = many(space | tab | newline);
	

	void set_whitespace(ptr< IParser<string, string> > parser)
	{
		whitespace = parser;
	}
}

#endif