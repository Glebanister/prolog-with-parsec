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

#ifndef PARSNIP_CHAR_COMBINATORS_H
#define PARSNIP_CHAR_COMBINATORS_H

#include "CharParser.h"
#include "StringConcatParser.h"

namespace Parsnip
{
/*
	tuple sequencing
*/
ptr<IParser<string, Tuple2<string, string> > > operator >> (char c, ptr<IParser<string, string> > p)
{
	return ch(c) >> p;
}


ptr<IParser<string, Tuple2<string, string> > > operator >> ( ptr<IParser<string, string> > p, char c)
{
	return p >> ch(c);
}

/*
	alternation
*/

ptr<IParser<string, string> > operator | (char c, ptr<IParser<string, string> > p)
{
	return ch(c) | p;
}


ptr<IParser<string, string> > operator | ( ptr<IParser<string, string> > p, char c)
{
	return p | ch(c);
}

/*
	concat sequence
*/

ptr<IParser<string, string> > operator + (char c, ptr<IParser<string, string> > p)
{
	return ch(c)+ p;
}


ptr<IParser<string, string> > operator + ( ptr<IParser<string, string> > p, char c)
{
	return p +  ch(c);
}


}
#endif