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

#ifndef CHOICE_PARSER_H
#define CHOICE_PARSER_H

#include "Memoizer.h"
#include "Parser.h"

namespace Parsnip
{
template <typename In, typename Out>
struct ChoiceParser : public IParser<In, Out>
{
    ChoiceParser(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b) : first(a), second(b)
    {
        this->setName("choice (" + a->getName() + " | " + b->getName() + ")");
    }

    /*
		parser choice must bypass any memoization that might
		happen in the IParser base class  
	*/
    virtual Result<Out> parse()
    {
        return eval();
    }

    virtual Result<Out> eval()
    {
        typename Reader<In>::IndexT lastPos = Reader<In>::pos();
        Result<Out> result = first->parse();
        if (result)
        {
            return result;
        }
        else
        {
            Reader<In>::set_pos(lastPos);
            Result<Out> result2 = second->parse();
            return result2;
        }
    }

    ptr<IParser<In, Out>> first;
    ptr<IParser<In, Out>> second;
};

template <typename In, typename Out>
inline ptr<IParser<In, Out>> choice(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b)
{
    typedef std::pair<ptr<IParser<In, Out>>, ptr<IParser<In, Out>>> Pair;
    static Memoizer<Pair, ptr<IParser<In, Out>>> memoizer;

    Pair p = std::make_pair(a, b);

    if (memoizer.contains(p))
        return memoizer.get(p);
    return memoizer.insert(p, new ChoiceParser<In, Out>(a, b));
}

template <typename In, typename Out>
inline ptr<IParser<In, Out>> operator|(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b)
{
    return choice(a, b);
}

} // namespace Parsnip

#endif
