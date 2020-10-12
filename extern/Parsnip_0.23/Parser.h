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

#ifndef PARSNIP_PARSER_H
#define PARSNIP_PARSER_H

#include "ParsnipConfig.h"
#include "ParsnipBase.h"
#include "Accumulators.h"
#include "Reducers.h"
#include "Result.h"
#include "Cache.h"

#include "Reader.h"
#include "ParsnipDebug.h"

namespace Parsnip
{
template <typename In, typename Out>
struct IParser
{	
	typedef  typename Reader<In>::IndexT PosT;
	typedef std::pair< Result<Out>, PosT> ResultPosPair;
	typedef Cache<In, Out, PosT, ResultPosPair > CacheT;
	typedef Maybe< ResultPosPair > MaybeValue; 

	virtual Result<Out> parse()
	{
		bool debug = trace || ParsnipConfig::traceAll;
		
		if (debug) 
		{
			typename Reader<In>::IndexT pos = Reader<In>::pos();

			ParsnipDebug::output_indent();

			std::cout << "(" << ParsnipDebug::indent << ") " << "Entering " <<  this->getName() << " at pos " << pos << " ";

			if (!Reader<In>::hasNext())
			{
				std::cout << "on end of stream ";
			}
			else if (pos >= 0)
			{
				std::cout << "on '" << Reader<In>::curr() <<  + "' ";
			}

			std::cout << std::endl;
			ParsnipDebug::increment_indent();
		}

		Result<Out> result; 

		switch(ParsnipConfig::strategy)
		{
		case (SIMPLE):
			result =  this->eval();
			break;

		case (PACKRAT):
			result = parse_packrat();
			break;
		}
		
		if (debug)
		{
			ParsnipDebug::decrement_indent();
		
			PosT pos = Reader<In>::pos();

			ParsnipDebug::output_indent();

			std::cout << "(" << ParsnipDebug::indent << ") " <<  "Exiting " << this->getName() << " at pos " << pos << " ";

			
			if (!Reader<In>::hasNext())
			{
				std::cout << "on end of stream ";
			}
			else if (pos >= 0)
			{
				std::cout << "on '" <<Reader<In>::curr() <<  + "' ";
			}

			if (result)
			{	
				std::cout << "OK "; 
			}
			else
			{	
				std::cout << "FAIL ";
			}

			std::cout << std::endl;			
		}

		return result;
	}

	Result<Out> parse_packrat()
	{
		PosT P = Reader<In>::pos();
		
		//see if there is a memo for this parser at the current position
		MaybeValue saved = cache.get(this, P);
		
		//if already memoized, return the memo
		if (saved) 
		{
			ResultPosPair rp = saved.get();
			Reader<In>::set_pos(rp.second);	
			return rp.first; 
		}

		//if not memoized do the parse
		Result<Out> ans = this->eval();
		
		//how far did we get?
		PosT newPos = Reader<In>::pos();

		//...and memoize it!
		cache.insert(this, P, std::make_pair(ans, newPos));

		return ans;
	}


	IParser() : trace(false)
	{
		this->setName(to_string(this));
	}

	virtual ~IParser() 
	{
		cache.removeParser(this);
	}
	

	virtual std::string getName()
	{
		return myName;
	}

	virtual void setName(const std::string& str)
	{
		myName = str;
	}

	virtual std::string toString(unsigned depth = 0)
	{
		std::string str;
		for (unsigned i =0; i < depth; ++i)
		{
			str += "\t";
		}
		str += getName();
		str += "\n";
		return str;
	}

	void setTrace(bool b) 
	{
		trace = b;
	}

protected:
	virtual Result<Out> eval() =0;


private:
	static CacheT cache;
	std::string myName;
	bool trace;
};

template <typename In, typename Out>
typename IParser<In, Out>::CacheT IParser<In, Out>::cache;

/* 
	use Parser<O, I>::type 
	because C++ doesn't yet
	have template typedefs
*/
template <typename In, typename Out>
struct Parser
{
	typedef ptr< IParser< In, Out > > type;
};


template<typename In, typename Out>
ptr< IParser<In, Out> > operator >>= (const std::string& name, ptr< IParser<In, Out> > p)
{
	p->setName(name);
	return p;
}

template<typename In, typename Out>
ptr< IParser<In, Out> > operator >>= (ptr< IParser<In, Out> > p, const std::string& name)
{
	p->setName(name);
	return p;
}


/*
template <typename In, typename Out>
struct ManyTillParser : public IParser
{
	ManyTillParser(ptr<IParser<In, Out> > s, ptr<IParser<In, Out> > t, int _min, int _max) : myParser(s), test(t), min(_min), max(_max) {}
	
	virtual Result<Out> eval()
	{
		std::string accumStr;
		iterator currIter= start;
		Result<Out> value, testOut;
		
		for (int i = 0; i < max; ++i)
		{
			if (currIter == finish)
			{
				return Result<Out>("unexpected end of input", start, FAIL);
			}
			
			// test to see if end condition has been met 
			testOut = test->parse(currIter, finish);
			if (testOut.state == OK)
			{
				if (i < min) break;
				else  return Result<Out>(accumStr, testOut.iter);
			}
			
			// if not at the end, continue scanning 
			value = myParser->parse(currIter, finish);
		
			if (value.state==OK) 
			{
				currIter = value.iter;
				accumStr += value.get();
			}
			else { break; }
		} 
		
		return Result<Out>("never reached expected input",  start, FAIL);
		

		
	}
	ptr<IParser<In, Out> >& myParser; 
	ptr<IParser<In, Out> > test;
	int min, max;
};


template <typename In, typename Out>
ptr<IParser<In, Out> > manyTill(ptr<IParser<In, Out> > a, ptr<IParser<In, Out> > test, int min = 0, int max = INT_MAX)
{
	return  ManyTillParser(a, test, min, max);
}


template <typename In, typename Out>
ptr<IParser<In, Out> > manyTill1(ptr<IParser<In, Out> > a, ptr<IParser<In, Out> > test)
{
	return manyTill(a, test, 1);
}


template <typename In, typename Out>
ptr<IParser<In, Out> > atleastTill(ptr<IParser<In, Out> > a, ptr<IParser<In, Out> > test, int min)
{
	return manyTill(a, test, min);
}

*/


}

#endif