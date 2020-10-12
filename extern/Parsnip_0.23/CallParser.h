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

#ifndef PARSNIP_CALL_PARSER_H
#define PARSNIP_CALL_PARSER_H

#include "Parser.h"
#include "Vector.h"
#include "Tuple.h"

namespace Parsnip
{

template< typename FnReturn, typename In, typename ParseReturn>
struct CallParser0 : public IParser< In, FnReturn>
{
	CallParser0(ptr<IParser<In, ParseReturn> > p, FnReturn (*fn)()) : myParser(p), myFn(fn) 
	{
		this->setName("call0");
	}
	
	Result<FnReturn> eval()
	{
		Result<ParseReturn> result = myParser->parse();
		if (result)
		{
			return Result<FnReturn>::succeed(myFn());
		}
		
		return Result<FnReturn>::fail();
	}
	
	ptr<IParser<In, ParseReturn> > myParser;
	FnReturn (*myFn)();
};

template<typename In, typename ParseReturn>
struct CallParser0<void, In, ParseReturn> : public IParser<In, void>
{
	CallParser0(ptr<IParser<In, ParseReturn> > p, void (*fn)()) : myParser(p), myFn(fn) 
	{
		this->setName("call0");
	}
	
	Result<void> eval()
	{
		Result<ParseReturn> result = myParser->parse();
		if (result)
		{
			myFn();
			return Result<void>::succeed();
		}
		
		return Result<void>::fail();
	}
	
	ptr<IParser<In, ParseReturn> > myParser;
	void (*myFn)();
};

template <typename FnReturn, typename In, typename ParseReturn>
ptr<IParser<In, FnReturn> > call0(FnReturn (*fn) (), ptr<IParser<In, ParseReturn> > parser)
{
	ptr<IParser<In, FnReturn> > p = new CallParser0<FnReturn, In, ParseReturn>(parser, fn);
	return p;
}


template< typename FnArg, typename FnReturn,  typename In, typename ParseReturn>
struct CallParser1 : public IParser<In, FnReturn>
{
	CallParser1(ptr<IParser<In, ParseReturn> > p, FnReturn (*fn)(FnArg)) : myParser(p), myFn(fn) 
	{
		this->setName("call1");
	}
	
	Result<FnReturn> eval()
	{
		Result<ParseReturn> result = myParser->parse();
		if (result) return Result<FnReturn>::succeed(myFn(result.data()));
		
		return Result<FnReturn>::fail();
	}
	
	ptr<IParser<In, ParseReturn> > myParser;
	FnReturn (*myFn)(FnArg);
};


template< typename FnArg,   typename In, typename ParseReturn>
struct CallParser1<void, FnArg,  In, ParseReturn> : public IParser<In, void>
{
	CallParser1(ptr<IParser<In, ParseReturn> > p, void (*fn)(FnArg)) : myParser(p), myFn(fn) 
	{
		p->setName("call1");
	}
	
	Result<void> eval()
	{
		Result<ParseReturn> result = myParser->parse();
		if (result)
		{
			myFn(result.data());
			return Result<void>();
		}
		return Result<void>::fail();
	}
	
	ptr<IParser<In, ParseReturn> > myParser;
	void (*myFn)(FnArg);
};


template <typename FnArg, typename FnReturn,  typename In, typename ParseReturn>
ptr<IParser<In, FnReturn> > call1(FnReturn (*fn) (FnArg), ptr<IParser<In, ParseReturn> > parser)
{
	ptr<IParser<In, FnReturn> > p =  new CallParser1< FnArg, FnReturn, In, ParseReturn>(parser, fn);
	return p;
}


/* 
	2-tuples pass 2 arguments to function
*/

template< typename FnReturn,  typename FnArg1, typename FnArg2, typename In, typename Out1, typename Out2>
struct CallParser2 : public IParser<In, FnReturn>
{
	typedef Tuple2<Out1, Out2> ParseTuple;

	CallParser2(ptr<IParser<In, ParseTuple> > p, FnReturn (*fn)(FnArg1, FnArg2)) : myParser(p), myFn(fn) 
	{
		this->setName("call2");
	}
	
	Result<FnReturn> eval()
	{
		Result<ParseTuple> result = myParser->parse();
		if (result) 
		{
			return Result<FnReturn>::succeed(myFn(result.data().first, result.data().second));
		}
		
		return Result<FnReturn>::fail();
	}
	
	ptr<IParser<In, ParseTuple> > myParser;
	FnReturn (*myFn)(FnArg1, FnArg2);
};


template< typename FnArg1, typename FnArg2,  typename In, typename Out1, typename Out2 >
struct CallParser2<void, FnArg1, FnArg2, In, Out1, Out2> : public IParser<In, void>
{	
	typedef Tuple2<Out1, Out2> ParseTuple;

	CallParser2(ptr<IParser<In, ParseTuple> > p, void (*fn)(FnArg1, FnArg2)) : myParser(p), myFn(fn) 
	{
		this->setName("call2");
	}
	
	Result<void> eval()
	{
		Result<ParseTuple> result = myParser->parse();
		if (result)
		{
			myFn(result.data().first, result.data().second());
			return Result<void>();
		}
		return Result<void>::fail();
	}
	
	ptr<IParser<In, ParseTuple> > myParser;
	void (*myFn)(FnArg1, FnArg2);
};


template < typename FnReturn, typename FnArg1, typename FnArg2, typename In, typename Out1, typename Out2>
ptr<IParser<In, FnReturn> > call2(FnReturn (*fn) (FnArg1, FnArg2), ptr<IParser<In, Tuple2<Out1, Out2> > > parser)
{
	ptr<IParser<In, FnReturn> > p = new CallParser2<FnReturn, FnArg1, FnArg2,  In, Out1, Out2>(parser, fn);
	return p;
}


/* 
	3-tuples pass 3 arguments to function
	
*/

template<typename FnReturn,  typename FnArg1, typename FnArg2, typename FnArg3,  typename In, typename Out1, typename Out2, typename Out3>
struct CallParser3 : public IParser<In, FnReturn>
{
	typedef Tuple3<Out1, Out2, Out3> ParseTuple;

	CallParser3(ptr<IParser<In, ParseTuple> > p, FnReturn (*fn)(FnArg1, FnArg2, FnArg3)) : myParser(p), myFn(fn) 
	{
		this->setName("call3");
	}
	
	Result<FnReturn> eval()
	{
		Result<ParseTuple> result = myParser->parse();
		if (result) 
		{
			return Result<FnReturn>::succeed(myFn(result.data().first, result.data().second, result.data().third));
		}
		
		return Result<FnReturn>::fail();
	}
	
	ptr<IParser<In, ParseTuple> > myParser;
	FnReturn (*myFn)(FnArg1, FnArg2, FnArg3);
};




template< typename FnArg1, typename FnArg2, typename FnArg3, typename In, typename Out1, typename Out2, typename Out3>
struct CallParser3<void, FnArg1, FnArg2, FnArg3, In,  Out1, Out2, Out3> : public IParser<In, void>
{	
	typedef Tuple3<Out1, Out2, Out3> ParseTuple;

	CallParser3(ptr<IParser<In, ParseTuple> > p, void (*fn)(FnArg1, FnArg2, FnArg3)) : myParser(p), myFn(fn) 
	{
		this->setName("call3");
	}
	
	Result<void> eval()
	{
		Result<ParseTuple> result = myParser->parse();
		if (result)
		{
			myFn(result.data().first, result.data().second(), result.data().third);
			return Result<void>();
		}
		return Result<void>::fail();
	}
	
	ptr<IParser<In, ParseTuple> > myParser;
	void (*myFn)(FnArg1, FnArg2, FnArg3);
};

template <typename FnReturn, typename FnArg1, typename FnArg2, typename FnArg3,  typename In, typename Out1, typename Out2, typename Out3>
ptr<IParser<In, FnReturn> > call3(FnReturn (*fn) (FnArg1, FnArg2, FnArg3), ptr<IParser<In, Tuple3<Out1, Out2, Out3>> > parser)
{
	ptr<IParser<In, FnReturn> > p =  new CallParser3<FnReturn, FnArg1, FnArg2, FnArg3, In, Out1, Out2, Out3>(parser, fn);
	return p;
}

}

#endif

