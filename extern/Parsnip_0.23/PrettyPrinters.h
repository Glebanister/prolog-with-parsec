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

#ifndef PARSNIP_PRETTY_PRINTERS_H
#define PARSNIP_PRETTY_PRINTERS_H

#include <map>
#include <ostream>
#include <typeinfo>

#include "Vector.h"

namespace Parsnip
{
template <typename T, unsigned int N>
inline std::ostream &operator<<(std::ostream &os, const Vector<T, N> &vec)
{
    //os << "vector <" << typeid(T).name() << "> " << endl;
    os << "vec: " << std::endl;
    for (unsigned int i = 0; i < (N - 1); ++i)
    {
        os << "[" << i << "] " << vec[i] << std::endl;
    }
    os << "[" << N - 1 << "] " << vec[N - 1];
    return os;
}

template <typename T1, typename T2>
inline std::ostream &operator<<(std::ostream &os, const std::map<T1, T2> &m)
{
    //os << "map < " << typeid(T1).raw_name() << ", " << typeid(T2).name() << " > " << endl;
    os << "map: " << std::endl;
    typename std::map<T1, T2>::const_iterator iter = m.begin();
    while (iter != m.end())
    {
        os << (*iter).first << " -> " << (*iter).second;
        ++iter;
        if (iter == m.end())
            break;
        os << std::endl;
    }
    return os;
}
} // namespace Parsnip

#endif
