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

#ifndef PARSNIP_EXCEPTION_H
#define PARSNIP_EXCEPTION_H

#include <exception>
#include <string>

namespace Parsnip
{
	struct ParsnipException : public std::exception
	{
		ParsnipException(const std::string& _error) throw()
			: errorMsg("Parser error: " + _error)
		{}

		~ParsnipException() throw() {}
    
		virtual const char* what() const throw()
		{
			return errorMsg.c_str(); 
		}
    

	private:
		std::string errorMsg;         
	};
}
#endif