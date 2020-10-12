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

#ifndef PARSNIP_SUCCEED_H
#define PARSNIP_SUCCEED_H

namespace Parsnip
{

template <typename In, typename Out>
struct SucceedParser : public IParser<In, Out>
{
	SucceedParser(const Out& _val) : val(_val) { }
		
protected:
		virtual Result<Out> eval()
		{
			return Result<Out>::succeed(val);
		}

private:
	Out val;
		
};

template <typename In, typename Out>
ptr< IParser<In, Out> > succeed (const Out& val)
{
	return new SucceedParser<In, Out>(val);
}


template <typename Out>
ptr< IParser<std::string, Out> > succeed (const Out& val)
{
	return new SucceedParser<std::string, Out>(val);
}


}
#endif