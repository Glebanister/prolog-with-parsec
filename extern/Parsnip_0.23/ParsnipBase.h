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

#ifndef PARSNIP_BASE_H
#define PARSNIP_BASE_H

#include <string>

#include "yasper.h"
using yasper::ptr;

#include <iostream>
using std::endl;
using std::cout;

#include <fstream>

#include <map>

//for conversions
#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iomanip>
#include <limits>
 

namespace Parsnip
{

 
 /* 
    to_string and fromString conversion taken from C++ FAQ Lite at 
    http://www.parashift.com/c++-faq-lite/misc-technical-issues.html 
*/
 
 class BadConversion : public std::runtime_error {
 public:
   BadConversion(const std::string& s)
     : std::runtime_error(s)
     { }
 };
 
 template<typename T>
 inline std::string to_string(const T& x)
 {
	std::ostringstream o;
	if ( !(o << x) )
    {
		throw BadConversion(std::string("to_string(") + typeid(x).name() + ")");
	}
	
	return o.str();
 }
    
 template<> inline std::string to_string<bool>(const bool& x)
 {
   std::ostringstream out;
   if ( !(out << std::boolalpha << x))
   {
		throw BadConversion(std::string("to_string(bool)"));
   }
   return out.str();
 }
 
 template<> inline std::string to_string<double>(const double& x)
 {
   const int sigdigits = std::numeric_limits<double>::digits10;
   std::ostringstream out;
   if ( !(out << std::setprecision(sigdigits) << x))
   {
		throw BadConversion(std::string("to_string(double"));
   }
   return out.str();
 }
 
 template<> inline std::string to_string<float>(const float& x)
 {
   const int sigdigits = std::numeric_limits<float>::digits10;
   std::ostringstream out;
   out << std::setprecision(sigdigits) << x;
   return out.str();
 }
 
 template<> inline std::string to_string<long double>(const long double& x)
 {
   const int sigdigits = std::numeric_limits<long double>::digits10;
   std::ostringstream out;
   if ( !(out << std::setprecision(sigdigits) << x))
   {
		throw BadConversion(std::string("to_string(float)"));
   }
   return out.str();
 }
 
 
template<typename T>
inline void fromStringByRef(const std::string& s, T& x, bool failIfLeftoverChars = true)
{
	std::istringstream i(s);
	char c;
	if ( !(i >> x ) || (failIfLeftoverChars && i.get(c)) ) throw BadConversion(s);
}


inline void fromStringByRef(const std::string& s, double& x, bool failIfLeftoverChars = true)
{
	const int sigdigits = std::numeric_limits<double>::digits10;
 
	std::istringstream i(s);
	char c;
	if ( !(i >> x) || (failIfLeftoverChars && i.get(c)) ) throw BadConversion(s);
}


/* converts from string to T returning T by value */
template<typename T>
inline T fromString(const std::string s)
{
	T x;
	fromStringByRef(s, x);
	return x;
}

void debug(std::string s)
{
	
	std::cout << std::endl <<  "DEBUG: " << s << endl;
}

template <typename T>
void debug(T t)
{
	debug(to_string(t));
}

std::string read_file(std::string name, bool preserve_newlines = true)
{
	std::string buf;
	std::string line;
	std::ifstream in("bla");
	while(std::getline(in,line))
	{
		buf += line;
		if (preserve_newlines)
		{
			buf += '\n';
		}
	}
	return buf;
}

}

#endif