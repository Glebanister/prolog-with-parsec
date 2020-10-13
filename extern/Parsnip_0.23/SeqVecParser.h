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

#ifndef PARSNIP_SEQ_VEC_PARSER_H
#define PARSNIP_SEQ_VEC_PARSER_H

#include "Parser.h"
#include "Vector.h"

namespace Parsnip
{

/*
	sequence of two vecs combines the vecs 
*/

template <typename In, typename Out, unsigned N1 = 1, unsigned N2 = 1>
struct SeqVecParser : public IParser<Vector<Out, N1 + N2>, In>
{
    typedef Vector<Out, N1> Vector1;
    typedef Vector<Out, N2> Vector2;
    typedef Vector<Out, N1 + N2> CombinedVector;

    SeqVecParser(ptr<IParser<Vector1, In>> a, ptr<IParser<Vector2, In>> b) : first(a), second(b) {}

protected:
    virtual Result<CombinedVector> eval()
    {
        Result<Vector1> result1 = first->parse();
        //Reader<In>::save();

        if (result1)
        {
            //	Reader<In>::discard();
            Result<Vector2> result2 = second->parse();

            if (result2)
            {
                return Result<CombinedVector>::succeed(result1.data() + result2.data());
            }

            return Result<CombinedVector>::fail();
        }

        //Reader<In>::restore();
        return Result<CombinedVector>::fail();
    }

    ptr<IParser<Vector1, In>> first;
    ptr<IParser<Vector2, In>> second;
};

/*
	sequence of non-vec and N-vec yields N+1 vec 
*/

template <typename In, typename Out, unsigned N1>
struct SeqVecParser<Out, In, N1, 1> : public IParser<Vector<Out, N1 + 1>, In>
{
    typedef Vector<Out, N1> Vector1;
    typedef Vector<Out, N1 + 1> CombinedVector;

    SeqVecParser(ptr<Parser<Vector1, In>> a, ptr<IParser<In, Out>> b) : first(a), second(b) {}

protected:
    virtual Result<CombinedVector> eval()
    {
        Result<Vector1> result1 = first->parse();
        if (result1)
        {
            Result<Out> result2 = second->parse();

            if (result2)
            {
                CombinedVector vec;
                for (unsigned i = 0; i < N1; ++i)
                {
                    vec[i] = result1.data()[i];
                }
                vec[N1] = result2.data();

                return Result<CombinedVector>(vec);
            }
            return Result<CombinedVector>::fail();
        }
        return Result<CombinedVector>::fail();
    }

    ptr<IParser<Vector1, In>> first;
    ptr<IParser<In, Out>> second;
};

/*
	sequence of N-vec and non-vec yields N+1 vec 
*/

template <typename In, typename Out, unsigned N2>
struct SeqVecParser<Out, In, 1, N2> : public IParser<Vector<Out, N2 + 1>, In>
{
    typedef Vector<Out, N2> Vector2;
    typedef Vector<Out, N2 + 1> CombinedVector;

    SeqVecParser(ptr<IParser<In, Out>> a, ptr<IParser<Vector2, In>> b) : first(a), second(b) {}

protected:
    virtual Result<CombinedVector> eval()
    {
        Result<Out> result1 = first->parse();
        if (result1)
        {
            Result<Vector2> result2 = second->parse();

            if (result2)
            {
                CombinedVector vec;
                vec[0] = result1.data();
                for (unsigned i = 1; i < N2; ++i)
                {
                    vec[i + 1] = result1.data()[i];
                }
                return Result<CombinedVector>(vec);
            }

            return Result<CombinedVector>::fail();
        }
        return Result<CombinedVector>::fail();
    }

    ptr<IParser<In, Out>> first;
    ptr<IParser<Vector2, In>> second;
};

/*
	sequence of two non-vecs yields a 2-vec
*/

template <typename In, typename Out>
struct SeqVecParser<Out, In, 1, 1> : public IParser<Vector<Out, 2>, In>
{
    typedef Vector<Out, 2> Vector2;

    SeqVecParser(ptr<IParser<In, Out>> a, ptr<IParser<In, Out>> b) : first(a), second(b) {}

protected:
    virtual Result<Vector2> eval()
    {
        Result<Out> result1 = first->parse();
        if (result1)
        {
            Result<Out> result2 = second->parse();

            if (result2)
            {
                Vector2 vec;
                vec[0] = result1.data();
                vec[1] = result2.data();

                return Result<Vector2>::succeed(vec);
            }
            return Result<Vector2>::fail();
        }
        return Result<Vector2>::fail();
    }

    ptr<IParser<In, Out>> first;
    ptr<IParser<In, Out>> second;
};
} // namespace Parsnip

#endif
