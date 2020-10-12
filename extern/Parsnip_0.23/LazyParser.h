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

#ifndef PARSNIP_LAZY_PARSER_H
#define PARSNIP_LAZY_PARSER_H

#include "FailParser.h"

namespace Parsnip
{

template <typename In, typename Out>
struct LazyParser : public IParser<In, Out>
{
	LazyParser() : target(new FailParser<In, Out>()) 
	{
		this->setName("lazy");
	}
	

	
	void setTarget(ptr<IParser<In, Out> > p)
	{
		target = p;
	}

	
protected:
	Result<Out> eval()
	{
		return target->parse();
	}
	
private:
	ptr<IParser<In, Out> > target;
};


template <typename In, typename Out>
IParser<In, Out>*  lazy()
{
	return new LazyParser<In, Out>();
}


template <typename Out>
IParser<std::string, Out>*  lazy()
{
	return lazy<std::string, Out>();
}

template <typename In, typename Out>
ptr<IParser<In, Out> > setLazy(ptr<IParser<In, Out> > delay, ptr<IParser<In, Out> > target)
{
	static_cast< ptr<LazyParser<In, Out> >  > (delay)->setTarget(target);
	return target; 
}


}

#endif