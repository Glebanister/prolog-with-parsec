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

#ifndef PARSNIP_READER_H
#define PARSNIP_READER_H

#include <string>
#include <stack>

namespace Parsnip
{
		

	template <typename Stream>
	struct Reader
	{
		typedef typename Stream::value_type Element;
		typedef typename Stream::size_type IndexT;

		static IndexT pos() { return _pos; } 
		static IndexT line()  { return _line; } 
		static bool hasNext() { return _pos < _stream.length(); }
		
		static IndexT longest()
		{
			if (_pos > _longest) 
			{
				return _pos;
			}
			else
			{ 
				return _longest; 
			}
		}

		static IndexT len()
		{ 
			return _stream.length(); 
		} 

		static Element curr()
		{
			return _stream[_pos];
		}

		static Element get()
		{
			return _stream[_pos++];
		}


//		static void set_stream(const Stream& _other) { _stream = _other; }

		static void init_stream(const Stream& _other) 
		{ 
			_stream = _other;
			_pos = 0;
			_longest = 0;
		}


		static void set_pos(IndexT p)
		{
			if (p > _longest) _longest = p;  		
			_pos = p;
		}


		//move current character back one position
		static void rewind()
		{
			_pos--;
		}

	private:
		static Stream _stream;
		static IndexT _pos;
		static IndexT _longest; 
		static IndexT _line;
	};

	//initialize Reader's static member data
	template <typename Stream>
	Stream Reader<Stream>::_stream;

	template <typename Stream>
	typename Reader<Stream>::IndexT Reader<Stream>::_pos;

	template <typename Stream>
	typename Reader<Stream>::IndexT Reader<Stream>::_line;

	template <typename Stream>
	typename Reader<Stream>::IndexT Reader<Stream>::_longest;
}

#endif