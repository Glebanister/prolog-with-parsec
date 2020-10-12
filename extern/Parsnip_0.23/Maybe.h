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

#ifndef PARSNIP_MAYBE_H
#define PARSNIP_MAYBE_H

/*
	Visual C++ compiler complains
	about some names exceeding 255
	characters...since the linker
	will inform us of name clashes
	we can disable the compiler warning
*/
#pragma warning (disable:4503)

#include <exception>
#include "ParsnipBase.h"

namespace Parsnip
{
template <typename T> 
struct Maybe
{
	Maybe() : valid(false) {}
	Maybe(const T& _data, bool _valid = true) :  data(_data), valid(_valid) {} 
	Maybe(const Maybe<T>& other) : data(other.data), valid(other.valid) { } 

	
	operator bool() const { return valid; }
	bool operator!()  const { return !valid; }
	
	const T& get() const
	{
		return data;
	}
	
private:
	bool valid;
	T data;
};

template<typename T>
Maybe<T> just(const T& t)
{
	return Maybe<T>(t, true);
}

template<typename T>
Maybe<T> nothing()
{
	return Maybe<T>();
}



/*
struct GotNothing : public std::exception
{
    GotNothing() throw() {}
    ~GotNothing() throw() {}
	
    const char* what() const throw()
    {
        return "[Maybe] Attempted to get Nothing";
    }
};
*/


}

#endif