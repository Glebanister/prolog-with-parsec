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


#ifndef PARSNIP_CACHE_BASE_H
#define PARSNIP_CACHE_BASE_H

#include <set>

namespace Parsnip
{

struct CacheBase
{
	typedef std::set<CacheBase*> CacheSet;
	typedef CacheSet::iterator CacheIter;
	
	virtual void clear()=0;
	
	static void add_cache(CacheBase* cache)
	{
		caches.insert(cache);
	}
	static void remove_cache(CacheBase* cache)
	{
		caches.erase(cache);
	}

	static void clear_all()
	{
		for (CacheIter iter = caches.begin(); iter != caches.end(); ++iter)
		{
			(*iter)->clear();
		}
	}

private:
	static CacheSet caches;
};

CacheBase::CacheSet CacheBase::caches;


}

#endif