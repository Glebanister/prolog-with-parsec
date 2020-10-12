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


#ifndef PARSNIP_CACHE_H
#define PARSNIP_CACHE_H

#include <map>
#include <set>
#include <memory>

#include "CacheBase.h"
#include "Maybe.h"
#include "Reader.h"

namespace Parsnip
{

template <typename In, typename Out>
struct IParser;

template<typename In, typename Out, typename DataT, typename ValueT> 
struct Cache : public CacheBase
{

	Cache() 
	{
		CacheBase::add_cache(this);
	}

	~Cache() 
	{
		CacheBase::remove_cache(this);
	}

	typedef IParser<In, Out>* ParserT;

	struct Key
	{
		Key(const ParserT& _parser, const DataT& _data) : parser(_parser), data(_data) {}

		ParserT parser;
		DataT data;

		bool operator< (const Key& other) const
		{
			return parser < other.parser || (parser == other.parser && data < other.data);
		}

	};

	typedef Key KeyT;

	typedef std::map<KeyT, ValueT> MapT;
	typedef typename MapT::iterator MapIter;

	typedef std::set<ParserT> SetT;
	typedef typename SetT::iterator SetIter;

	Maybe<ValueT> get(ParserT parser_ptr, const DataT& data)
	{
		//check is input has already been memoized
		MapIter iter = memos.find(Key(parser_ptr, data));
		
		//if key not in map, return an empty Maybe<>
		if (iter == memos.end()) return nothing<ValueT>();

		return just((*iter).second);
	}
	
	void insert(ParserT parser_ptr, const DataT& data, const ValueT& value)
	{
		KeyT key(parser_ptr, data);

		//check is key already exists
		MapIter iter = memos.find(key);
		
		//if it's a duplicate, delete the original
		if (iter != memos.end()) memos.erase(iter);

		memos.insert(std::make_pair(key, value));

		//if parser_ptr is a duplicate, std::set won't insert it
		unique_parsers.insert(parser_ptr);
	}

	void removeParser(ParserT parser_ptr)
	{
		if (!memos.empty())
		{
			MapIter iter = memos.begin(); 
			MapIter end = memos.end();
			
			while (iter != end)
			{
				KeyT currKey = (*iter).first;
				if (currKey.parser == parser_ptr)
				{
					memos.erase(iter++);
				}
				else
				{
					++iter;
				}
			} 
		}

		if (!unique_parsers.empty())
		{
			unique_parsers.erase(parser_ptr);
		}
	}

	
	virtual void clear()
	{
		unique_parsers.clear();
		memos.clear();
	}

	MapT memos;
	SetT unique_parsers;
};


}

#endif