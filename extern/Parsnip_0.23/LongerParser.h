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

#ifndef PARSNIP_LONGER_H
#define PARSNIP_LONGER_H

#include "Parser.h"

namespace Parsnip
{
template <typename In, typename Out>
struct LongerParser : public IParser<In, Out>
{
    LongerParser(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b) : first(a), second(b)
    {
        this->setName("longer");
    }

    virtual Result<Out> eval()
    {
        typename Reader<In>::IndexT startPos = Reader<In>::pos();

        Result<Out> result1 = first->parse();
        typename Reader<In>::IndexT firstPos = Reader<In>::pos();

        Reader<In>::set_pos(startPos);

        Result<Out> result2 = second->parse();
        typename Reader<In>::IndexT secondPos = Reader<In>::pos();

        if (result1 && result2)
        {
            if (firstPos > secondPos)
            {
                Reader<In>::set_pos(firstPos);
                return result1;
            }
            else
            {
                return result2;
            }
        }
        else if (result1)
        {
            Reader<In>::set_pos(firstPos);
            return result1;
        }
        else
        {
            return result2;
        }
    }

    ptr<IParser<In, Out>> first;
    ptr<IParser<In, Out>> second;
};

template <typename In, typename Out>
inline ptr<IParser<In, Out>> longer(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b)
{
    return new LongerParser<In, Out>(a, b);
}

template <typename In, typename Out>
inline ptr<IParser<In, Out>> longest(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b)
{
    return longer(a, b);
}

template <typename In, typename Out>
inline ptr<IParser<In, Out>> longest(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b, ptr<IParser<In, Out>> c)
{
    return longer(longer(a, b), c);
}

template <typename In, typename Out>
inline ptr<IParser<In, Out>> longest(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b, ptr<IParser<In, Out>> c, ptr<IParser<In, Out>> d)
{
    return longer(longer(a, b), longer(c, d));
}

} // namespace Parsnip

#endif