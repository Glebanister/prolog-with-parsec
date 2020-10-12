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

#ifndef OPTIONAL_PARSER_H
#define OPTIONAL_PARSER_H

#include "Parser.h"

namespace Parsnip
{

template <typename In, typename Out>
struct OptionalParser : public IParser<In, Out>
{
	OptionalParser(ptr<IParser<In, Out> > p, const Out& _default) : myParser(p), defaultVal(_default) 
	{
		this->setName("optional");
	}
	Result<Out> eval()
	{
		Result<Out> result = myParser->parse();
		
		/*
			if parse succeeded, return its result
			otherwise pretend like it succeeded
		*/
		return result ? result :  Result<Out>::succeed(defaultVal);
	}
	
	ptr<IParser<In, Out> > myParser;
	Out defaultVal;
};

template<typename In, typename Out>
ptr<IParser<In, Out> > optional(ptr<IParser<In, Out> > p, const Out& default_val)
{
	return new OptionalParser<In, Out>(p, default_val);
}

}

#endif