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


#ifndef PARSNIP_TUPLE_H
#define PARSNIP_TUPLE_H

namespace Parsnip
{

	template <typename T1, typename T2>
	struct Tuple2
	{
		typedef T1 FirstType;
		typedef T2 SecondType;
		
		Tuple2() {}
		Tuple2(const T1& _first, const T2& _second) : first(_first), second(_second) {}

		T1 first;
		T2 second;
	};

	template <typename T1, typename T2, typename T3>
	struct Tuple3
	{
		typedef T1 FirstType;
		typedef T2 SecondType;
		typedef T3 ThirdType;
		
		Tuple3() {}
		Tuple3(const T1& _first, const T2& _second, const T3& _third)
			: first(_first), second(_second), third(_third) {}

		T1 first;
		T2 second;
		T3 third;
	};

	template <typename T1, typename T2, typename T3, typename T4>
	struct Tuple4
	{
		typedef T1 FirstType;
		typedef T2 SecondType;
		typedef T3 ThirdType;
		typedef T4 FourthType;

		Tuple4() {}
		Tuple4(const T1& _first, const T2& _second, const T3& _third, const T4& _fourth)
			: first(_first), second(_second), third(_third), fourth(_fourth) {}

		T1 first;
		T2 second;
		T3 third;
		T4 fourth;
	};


}

#endif