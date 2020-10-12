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

#ifndef PARSNIP_PARSE_RESULT_H
#define PARSNIP_PARSE_RESULT_H

#include "yasper.h"
using yasper::ptr;

namespace Parsnip
{
	template <typename Out>
	struct ParseResult
	{
		typedef int IndexT;
		
		ParseResult() : myData(0), myCurrPos(0), myLastPos(0) {}

		ParseResult(IndexT _currPos, IndexT _lastPos ) : myData(0), myCurrPos(_currPos), myLastPos(_lastPos)
		{ }
		
		ParseResult(const Out& _data, IndexT _currPos, IndexT _lastPos ) : myData(new Out(_data)), myCurrPos(_currPos), myLastPos(_lastPos)
		{ }

		Out& data()
		{
			return (*myData);
		}

		bool input_consumed()
		{
			return myCurrPos == myLastPos;
		}
		
		bool parse_finished()
		{
			return myData && input_consumed();
		}

		IndexT parse_position()
		{
			return myCurrPos;
		}

	private:
		IndexT myCurrPos;
		IndexT myLastPos;
		ptr<Out> myData;
	};
}

#endif