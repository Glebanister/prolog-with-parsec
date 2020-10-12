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

#ifndef PARSNIP_DEBUG_H
#define PARSNIP_DEBUG_H

#include <iostream>

namespace Parsnip
{

	struct ParsnipDebug
	{
		static unsigned indent;

		static void increment_indent()
		{
			++indent;
		}

		static void decrement_indent()
		{
			--indent;
		}

		static void output_indent()
		{
			for (unsigned i = 0; i < indent; ++i)
			{
				std::cout << "   ";
			}
		}
	};

	unsigned ParsnipDebug::indent; 

}

#endif