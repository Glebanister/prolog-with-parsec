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

#ifndef PARSNIP_MAKE_PARSER_H
#define PARSNIP_MAKE_PARSER_H

#include "Parser.h"

namespace Parsnip
{

	/*  A CreateParser accepts another parser (P In -> Out) and 
	uses its return value to create a CreateType,
	which is cast as a ReturnType.
	In short: mapping In => Out => CreateType => ReturnType
*/

	template <typename CreateType, typename ReturnType, typename In, typename Out>
	struct CreateParser : public IParser<In, ReturnType>
	{
		typedef Result<ReturnType> NewOut;

		CreateParser(ptr<IParser<In, Out>> p) : myParser(p) {}

		NewOut eval()
		{
			Result<Out> result = myParser->parse();

			if (result)
			{
				return NewOut::succeed(new CreateType(result.data()));
			}

			return NewOut::fail();
		}

		ptr<IParser<In, Out>> myParser;
	};

	/*
	Constructor takes no arguments
*/

	template <typename CreateType, typename ReturnType, typename In>
	struct CreateParser<CreateType, ReturnType, In, void> : public IParser<In, ReturnType>
	{
		typedef Result<ReturnType> NewOut;

		CreateParser(ptr<IParser<In, void>> p) : myParser(p) {}

		NewOut eval()
		{
			if (myParser->parse())
			{
				return NewOut::succeed(new CreateType);
			}

			return NewOut::fail();
		}

		ptr<IParser<In, void>> myParser;
	};

	/*
	Constructor takes 2 arguments
*/

	template <typename CreateType, typename ReturnType, typename In, typename Out1, typename Out2>
	struct CreateParser<CreateType, ReturnType, In, Tuple2<Out1, Out2>> : public IParser<In, ReturnType>
	{
		typedef Result<ReturnType> NewOut;
		typedef Tuple2<Out1, Out2> TupleT;

		CreateParser(ptr<IParser<In, TupleT>> p) : myParser(p) {}

		NewOut eval()
		{

			Result<TupleT> result = myParser->parse();
			if (result)
			{
				TupleT tuple = result.data();
				return NewOut::succeed(new CreateType(tuple.first, tuple.second));
			}

			return NewOut::fail();
		}

		ptr<IParser<In, TupleT>> myParser;
	};

	/*
	Constructor takes 3 arguments
*/

	template <typename CreateType, typename ReturnType, typename In, typename Out1, typename Out2, typename Out3>
	struct CreateParser<CreateType, ReturnType, In, Tuple3<Out1, Out2, Out3>> : public IParser<In, ReturnType>
	{
		typedef Result<ReturnType> NewOut;
		typedef Tuple3<Out1, Out2, Out3> TupleT;

		CreateParser(ptr<IParser<In, TupleT>> p) : myParser(p) {}

		NewOut eval()
		{

			Result<TupleT> result = myParser->parse();
			if (result)
			{
				TupleT tuple = result.data();
				return NewOut::succeed(new CreateType(tuple.first, tuple.second, tuple.third));
			}

			return NewOut::fail();
		}

		ptr<IParser<In, TupleT>> myParser;
	};

	/*
	Constructor takes 4 arguments
*/

	template <typename CreateType, typename ReturnType, typename In, typename Out1, typename Out2, typename Out3, typename Out4>
	struct CreateParser<CreateType, ReturnType, In, Tuple4<Out1, Out2, Out3, Out4>> : public IParser<In, ReturnType>
	{
		typedef Result<ReturnType> NewOut;
		typedef Tuple4<Out1, Out2, Out3, Out4> TupleT;

		CreateParser(ptr<IParser<In, TupleT>> p) : myParser(p) {}

		NewOut eval()
		{

			Result<TupleT> result = myParser->parse();
			if (result)
			{
				TupleT tuple = result.data();
				return NewOut::succeed(new CreateType(tuple.first, tuple.second, tuple.third, tuple.fourth));
			}

			return NewOut::fail();
		}

		ptr<IParser<In, TupleT>> myParser;
	};

	template <typename CreateType, typename ReturnType, typename In, typename Out>
	ptr<IParser<In, ReturnType>> create(ptr<IParser<In, Out>> parser)
	{
		return new CreateParser<CreateType, ReturnType, In, Out>(parser);
	}

	/*
	if return type is omitted it's assumed to be a pointer to CreateType
*/

	template <typename CreateType, typename In, typename Out>
	ptr<IParser<In, CreateType *>> create(ptr<IParser<In, Out>> parser)
	{
		return new CreateParser<CreateType, CreateType *, In, Out>(parser);
	}

} // namespace Parsnip

#endif