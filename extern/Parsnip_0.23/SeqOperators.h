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

#ifndef PARSNIP_SEQ_PARSER_H
#define PARSNIP_SEQ_PARSER_H

#include "SeqUseParser.h"
#include "SeqTupleParser.h"
#include "SeqVoidParser.h"

namespace Parsnip
{

/* 
	void and single item 
*/


template <typename In, typename Out>
ptr<IParser<In, Out> > seq( ptr<IParser<In, void> > a, ptr<IParser<In, Out> > b)
{
   ptr<IParser<In, Out> > p =  new SeqUseSecondParser<In, Out>(a,b); 
   p->setName("seq");
   return p;
} 


template <typename In, typename Out>
ptr<IParser<In, Out> > operator >>( ptr<IParser<In, void> > a, ptr<IParser<In, Out> > b)
{
	return seq(a,b);
}


/*
	single item and void 
*/

template <typename In, typename Out>
ptr<IParser<In, Out> > seq( ptr<IParser<In, Out> > a, ptr<IParser<In, void> > b)
{
    ptr<IParser<In, Out> > p =  new SeqUseFirstParser<In, Out>(a,b); 
	p->setName("seq");
	return p;

} 


template <typename In, typename Out>
ptr<IParser<In, Out> > operator >>( ptr<IParser<In, Out> > a, ptr<IParser<In, void> > b)
{
	return seq(a,b);
}

/* 
	void and void
*/

template <typename In>
ptr<IParser<In, void> > seq( ptr<IParser<In, void> > a, ptr<IParser<In, void> > b)
{
    ptr<IParser<In, void> > p =  new SeqVoidParser<In>(a,b); 
	p->setName("seq");
	return p;

} 


template <typename In>
ptr<IParser<In, void> > operator >>( ptr<IParser<In, void> > a, ptr<IParser<In, void> > b)
{
	return seq(a,b);
}

/*
	two items of different types combined into a tuple using >>
*/

template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2>> > seq (ptr<IParser<In, Out1> > a, ptr<IParser<In, Out2> > b)
{
	ptr<IParser<In, Tuple2<Out1, Out2>> > p =  new Seq1_1Parser<In, Out1, Out2>(a, b);
	p->setName("seq");
	return p;

}


template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2>> > operator >> (ptr<IParser<In, Out1> > a, ptr<IParser<In, Out2> > b)
{
	return seq(a,b);

}

/*
	a 2-tuple and an item => 3-tuple 
*/

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > seq (ptr< IParser<In, Tuple2<Out1,Out2> > > a, ptr<IParser<In,Out3> > b)
{
	return new Seq2_1Parser<In, Out1, Out2, Out3>(a, b);
}


template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > operator >> (ptr< IParser<In, Tuple2<Out1,Out2> > > a, ptr<IParser<In,Out3> > b)
{
	return seq(a,b);
}

/*
	an item and a 2-tuple => 3-tuple
*/

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > seq ( ptr<IParser<In, Out1> > a, ptr<IParser<In, Tuple2<Out2,Out3> > > b)
{
	return new Seq1_2Parser<In, Out1, Out2, Out3>(a, b);
}


template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > operator >> ( ptr<IParser<In, Out1> > a, ptr<IParser<In, Tuple2<Out2,Out3> > > b)
{
	return seq(a,b);
}

/* a void and a 2-tuple */

template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2> > > seq (ptr<IParser<In,void> > a, ptr< IParser<In, Tuple2<Out1,Out2> > > b)
{
	return new SeqUseSecondParser<In, Tuple2<Out1, Out2> >(a,b);
}

template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2> > > operator >> (ptr<IParser<In,void> > a, ptr< IParser<In, Tuple2<Out1,Out2> > > b)
{
	seq(a,b);
}

/* a 2-tuple and a void => 2-tuple */

template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2> > > seq ( ptr< IParser<In, Tuple2<Out1,Out2> > > a, ptr<IParser<In,void> > b)
{
	return new SeqUseFirstParser<In, Tuple2<Out1, Out2> >(a,b);
}

template <typename In, typename Out1,  typename Out2>
ptr<IParser<In, Tuple2<Out1, Out2> > > operator >> ( ptr< IParser<In, Tuple2<Out1,Out2> > > a, ptr<IParser<In,void> > b)
{
	return seq(a,b);
}

/* a void and a 3-tuple */

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > seq (ptr<IParser<In,void> > a, ptr< IParser<In, Tuple3<Out1,Out2,Out3> > > b)
{
	return new SeqUseSecondParser<In, Tuple3<Out1, Out2, Out3> >(a,b);
}

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2, Out3> > > operator >> (ptr<IParser<In,void> > a, ptr< IParser<In, Tuple3<Out1,Out2,Out3> > > b)

{
	seq(a,b);
}

/* a 3-tuple and a void => 3-tuple */

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2,Out3> > > seq ( ptr< IParser<In, Tuple3<Out1,Out2,Out3> > > a, ptr<IParser<In,void> > b)
{
	return new SeqUseFirstParser<In, Tuple3<Out1, Out2, Out3> >(a,b);
}

template <typename In, typename Out1,  typename Out2, typename Out3>
ptr<IParser<In, Tuple3<Out1, Out2,Out3> > > operator >> ( ptr< IParser<In, Tuple3<Out1,Out2,Out3> > > a, ptr<IParser<In,void> > b)
{
	return seq(a,b);
}

}
#endif