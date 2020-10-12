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

#ifndef PARSNIP_RESULT_H
#define PARSNIP_RESULT_H

#include "ParsnipBase.h"
#include "Maybe.h"
namespace Parsnip
{

template <typename Out>
struct Result
{	
	//constructor without data signifies a failed result
	Result () : myData(nothing<Out>()) {}

	Result(const Result& other) : myData(other.myData)
	{
		
	}

    operator bool()  const { return myData; }
	bool operator!() const { return !myData;  }
	
	static Result fail()
	{
		return Result();
	}
	
	static Result succeed(const Out& _data)
	{
		return Result(_data);
	}

	const Out& data() 
	{
		return myData.get();
	}
	
private:
	Maybe<Out> myData;

	//constructor with data is a successful result
	Result(Out _data) :  myData(just(_data)) {}

};

/* 
	if Out type if void no need to store any data 
*/

template <>
struct Result<void>
{	
	Result() : success(false) {}

	Result(const Result& other) : success(other.success) {}
	
	static Result fail()
	{
		return Result(false); 
	}
	
	
	static Result succeed()
	{
		return Result(true); 
	}
	
	
    operator bool() const  { return success; }
	bool operator!() const { return !success; }

	bool success;


private:
	
	Result(bool _succ) : success(_succ) {}
};


}


#endif
