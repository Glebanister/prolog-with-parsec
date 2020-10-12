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

#ifndef PARSNIP_SEQ_VEC_OPERATORS_H
#define PARSNIP_SEQ_VEC_OPERATORS_H

#include "SeqVecParser.h"

namespace Parsnip
{

/*
	two items of the same type can be made into a vector using &&
*/

template <typename In, typename Out>
ptr<IParser<In, Vector<Out, 2> > > seq_vec ( ptr<IParser<In, Out> > a, ptr<IParser<In, Out> > b)
{
   return new SeqVecParser<In, Out, 1, 1>(a,b); 
} 


/*
	two Vectors of the same type combined with &&
*/

template <typename In, typename Out, unsigned N1, unsigned N2>
ptr<IParser<Vector<Out, N1 + N2>, In> > seq_vec ( ptr<IParser<In, Vector<Out, N1> > > a, ptr<IParser<In, Vector<Out, N2> > > b)
{
   return  new SeqVecParser<In, Out, N1, N2>(a,b); 
} 

/*
	N-vector and single item of the same type
*/

template <typename In, typename Out, unsigned N>
ptr<IParser < In, Vector <Out, N + 1> > > seq_vec ( ptr< IParser< In, Vector<Out, N>> > a, ptr<Parser<In, Out> > b)
{
   return new SeqVecParser<In, Out, N, 1>(a,b); 
} 

/*
	single item and N-vector of the same type
*/

template <typename In, typename Out, unsigned N>
ptr<IParser<In, Vector<Out, N + 1> > > seq_vec( ptr<Parser<In, Out> > a, ptr<IParser<In, Vector<Out, N>> > b)
{
   return new SeqVecParser<In, Out, 1, N>(a,b); 
} 

}

#endif