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

#ifndef PARSNIP_MEMOIZER_H
#define PARSNIP_MEMOIZER_H

#include <map>
#include "CacheBase.h"
namespace Parsnip
{

	template <typename S, typename T>
	struct Memoizer
	{
		Memoizer()
		{
		}

		~Memoizer()
		{
		}

		typedef typename std::map<S, T>::iterator MapIter;

		bool contains(const S &input)
		{
			//check is input has already been memoized
			MapIter iter = memos.find(input);

			return !(iter == memos.end());
		}

		const T &insert(const S &input, const T &result)
		{
			memos.insert({input, result});
			return result;
		}

		const T &get(const S &input)
		{
			MapIter iter = memos.find(input);
			return (*iter).second;
		}

		T memo(S input, const T &result)
		{

			//check is input has already been memoized
			MapIter iter = memos.find(input);

			//if new, add it to the cache
			if (iter == memos.end())
			{
				memos.insert(std::make_pair<S, T>(input, result));
				return result;
			}

			return (*iter).second;
		}

		virtual void clear()
		{
			memos.clear();
		}

	private:
		std::map<S, T> memos;
	};

} // namespace Parsnip

#endif