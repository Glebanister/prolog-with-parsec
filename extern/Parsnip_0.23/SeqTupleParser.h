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

#ifndef SEQ_TUPLE_PARSER_H
#define SEQ_TUPLE_PARSER_H

#include "Parser.h"
#include "Tuple.h"

namespace Parsnip
{
/*
	sequence of two non-tuples yields a 2-tuple
*/

template <typename In, typename  Out1, typename Out2>
struct Seq1_1Parser : public IParser<In, Tuple2<Out1,Out2> >
{	
	Seq1_1Parser( ptr<IParser<In, Out1 > > a, ptr<IParser<In, Out2> > b ) : first(a), second(b) 
	{
		this->setName("seq");
	}
	
protected:
	virtual Result< Tuple2<Out1, Out2> > eval()
	{

		Result<Out1> result1 = first->parse();

		//unsigned lastPos = Reader<In>::pos();
		
		if(result1)
		{ 
			Result<Out2> result2 = second->parse();
		
			if (result2) 
			{
				Tuple2<Out1, Out2> tuple;
				tuple.first = result1.data();
				tuple.second = result2.data();

				return Result<Tuple2<Out1, Out2> >::succeed(tuple);
			}
			else
			{
				return Result<Tuple2<Out1, Out2> >::fail();
			}
		}
		//if we fail to parse...do we restore the position?
		
		return Result<Tuple2<Out1, Out2> >::fail();
	}
	
	ptr<IParser<In, Out1> > first;
	ptr<IParser<In, Out2> > second;
};

/*
	sequence of 2-tuple and non-tuple yields a 3-tuple
*/

template <typename In, typename Out1, typename Out2, typename Out3>
struct Seq2_1Parser : public IParser<In, Tuple3<Out1,Out2,Out3> >
{	
	Seq2_1Parser( ptr<IParser<In, Tuple2<Out1, Out2> > > a, ptr< IParser<In, Out3> > b ) : first(a), second(b) 
	{
		this->setName("seq");
	}
	
protected:
	virtual Result<Tuple3<Out1, Out2, Out3> > eval()
	{
		Result<Tuple2<Out1, Out2> > result1 = first->parse();
		if(result1)
		{ 
			Result<Out3> result2 = second->parse();
		
			if (result2) 
			{
				Tuple3<Out1, Out2, Out3> tuple(result1.data().first, result1.data().second, result2.data());
	
				return Result<Tuple3<Out1, Out2, Out3> >::succeed(tuple);
			}
			else
			{
				return Result< Tuple3<Out1, Out2, Out3> >::fail();
			}
		}
		return Result<Tuple3<Out1, Out2, Out3> >::fail();	
	}
	
	ptr<IParser<In, Tuple2<Out1, Out2> > > first;
	ptr< IParser<In, Out3> >  second;
};


/*
	sequence of non-tuple and 2-tuple yields a 3-tuple
*/

template <typename In, typename Out1, typename Out2, typename Out3>
struct Seq1_2Parser : public IParser<In, Tuple3<Out1,Out2,Out3> >
{	
	Seq1_2Parser( ptr<IParser<In, Out1> > a, ptr<IParser<In, Tuple2<Out2, Out3> > > b ) : first(a), second(b) 
	{
		this->setName("seq");
	}
	
protected:
	virtual Result<Tuple3<Out1, Out2, Out3> > eval()
	{
		Result<Out1> result1 = first->parse();
		if(result1)
		{ 
			Result< Tuple2<Out2, Out3> > result2 = second->parse();
		
			if (result2) 
			{
				Tuple3<Out1, Out2, Out3> tuple(result1.data(), result2.data().first, result2.data().second);
	
				return Result< Tuple3<Out1, Out2, Out3> >::succeed(tuple);
			}
			else
			{	
				return Result< Tuple3<Out1, Out2, Out3> >::fail();
			}
		}

		return Result< Tuple3<Out1, Out2, Out3> >::fail();
		
	}
	
	ptr< IParser<In, Out1> > first;
	ptr<IParser<In, Tuple2<Out2, Out3> > > second;
};

}

#endif