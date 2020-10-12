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

#ifndef PARSNIP_ACCUMULATORS_H
#define PARSNIP_ACCUMULATORS_H

#include "ParsnipBase.h"
#include "Vector.h"
#include "Tuple.h"

#include <list>
#include <map>

namespace Parsnip
{

template <typename OutType, typename InType>
struct Accumulator
{
	virtual void accum(const InType& x)=0;
	virtual OutType result()=0;
	virtual ~Accumulator() {}
	
	using ResultType = OutType;

	bool empty;
};


template <typename T>
struct Concat : public Accumulator<T, T>
{	

	virtual void accum(const T& s) 
	{
		if (this->empty)
		{
			value = s;
			this->empty = false;
		}
		else value += s; 
	}
	
	virtual T result() { return value; }
	T value;
	bool empty = false;
};

typedef Concat<std::string> ConcatString;


template <typename InType>
struct Ignore : public Accumulator<void, InType>
{
	virtual void accum(const InType& i) { /* do nothing */}
	virtual void result() { /* do nothing */}

};

template <typename InType>
struct BuildList : public Accumulator<std::list<InType>, InType>
{
	virtual void accum(const InType& i) { myList.push_back(i); }
	virtual std::list<InType> result() { return myList; }
	
private:
	std::list<InType> myList;
};

template<typename T>
struct BuildMapFromVec : public Accumulator<std::map<T, T>, Vector<T, 2>>
{
	virtual void accum(const Vector<T, 2>& t) 
	{ 
		myMap.insert(std::make_pair(t[0], t[1])); 
	}

	virtual std::map<T,T> result() { return myMap; }

private:
	std::map<T,T> myMap;
};

template<typename T1, typename T2>
struct BuildMap : public Accumulator<std::map<T1, T2>, Tuple2<T1, T2> >
{
	virtual void accum(const Tuple2<T1, T2>& t) 
	{ 
		myMap.insert(std::make_pair(t.first, t.second)); 
	}

	virtual std::map<T1,T2> result() { return myMap; }

private:
	std::map<T1,T2> myMap;
};

template <typename OutType , typename InType>
struct Add : public Accumulator<OutType, InType>
{	
	Add()  {}

	virtual void accum(const InType& s) 
	{
		if (this->empty)
		{
			value = s;
			this->empty = false;
		}
		else value += s; 
	}
	virtual OutType result() { return value; }
private:
	OutType value;
};

typedef Add<double, double> AddDouble;
typedef Add<int, int> AddInt;

template <typename OutType , typename InType>
struct Sub : public Accumulator<OutType, InType>
{	
	Sub()  {}
	virtual void accum(const InType& s) 
	{ 
		if (this->empty)
		{
			value = s;
			this->empty = false;
		}
		else value -= s; 
	}
	virtual OutType result() { return value; }
private:
	OutType value;
};

typedef Sub<double, double> SubDouble;
typedef Sub<int, int> SubInt;


template <typename OutType , typename InType>
struct Mult : public Accumulator<OutType, InType>
{	
	Mult ()  {}

	virtual void accum(const InType& s) 
	{ 
		if (this->empty)
		{
			value = s;
			this->empty = false;
		}
		else value *= s; 
	}
	virtual OutType result() { return value; }
private:
	OutType value;
};

typedef Mult<double, double> MultDouble;
typedef Mult<int, int> MultInt;

template <typename OutType , typename InType>
struct Div : public Accumulator<OutType, InType>
{	
	Div()  {}
	virtual void accum(const InType& s) 
	{ 
		if (this->empty)
		{
			value = s;
			this->empty = false;
		}
		else value *= s; 
	}
	virtual OutType result() { return value; }

private:
	OutType value;
};

typedef Div<double, double> DivDouble;
typedef Div<int, int> DivInt;

}
#endif