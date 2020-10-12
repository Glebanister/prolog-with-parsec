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


/*
	July 25th: renamed to Vector

	Modified by Alex Rubinsteyn (alex.rubinsteyn@gmail.com)
	on May 16th, 2007 for use in the Parsnip parser
	combinator library.
		- changed size_t and int to unsigned
		- removed assertions
		- dropped inclusion of std namespace
		- wrapped vec in Parsnip namespace
		- added operator+ for combining vecs
*/


/*   _______              __
    / ___/ /  ___  __ _  / /  ___
   / /__/ _ \/ _ \/  V \/ _ \/ _ \
   \___/_//_/\___/_/_/_/_.__/\___/
*/
// CHOMBO Copyright (c) 2000-2004, The Regents of the University of
// California, through Lawrence Berkeley National Laboratory (subject to
// receipt of any required approvals from U.S. Dept. of Energy).  All
// rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// (2) Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// (3) Neither the name of Lawrence Berkeley National Laboratory, U.S.
// Dept. of Energy nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// You are under no obligation whatsoever to provide any bug fixes,
// patches, or upgrades to the features, functionality or performance of
// the source code ("Enhancements") to anyone; however, if you choose to
// make your Enhancements available either publicly, or directly to
// Lawrence Berkeley National Laboratory, without imposing a separate
// written license agreement for such Enhancements, then you hereby grant
// the following license: a non-exclusive, royalty-free perpetual license
// to install, use, modify, prepare derivative works, incorporate unsignedo
// other computer software, distribute, and sublicense such Enhancements or
// derivative works thereof, in binary and source code form.
//
// TRADEMARKS. Product and company names mentioned herein may be the
// trademarks of their respective owners.  Any rights not expressly granted
// herein are reserved.
//

#ifndef CHOMBO_TUPLE_H
#define CHOMBO_TUPLE_H

namespace Parsnip
{

//
/// Ordered Vectors for Types T
/**

  This class represents ordered vecs of some user-specified concrete
  type T for N > 0. The type T must have a default constructor.  If the
  non-default constructor, copy constructor, or copy assignment operator
  are used, T must also have a copy constructor.
*/

template <class T, unsigned N>
class Vector
{
public:

    /**: The default constructor.  For user-defined types T, the
               default constructor for T will be run on each of the N
               objects in the Vector.  For builtin (intrinsic) types,
               the values in the Vector will be garbage.
    */
    Vector ();

    /**: Constructs a Vector, initializing the elements in the Vector
               with the corresponding elements in the vector v.  This assumes
               that v contains at least N elements of type T -- an assumption
               that is NOT checked.  For user-defined types, T must have a
               well-defined and accessible copy constructor.
    */
  //explicit Vector (const T* v);
    //
    // The copy constructor.
    //
    Vector (const Vector& rhs);
    //
    // The copy assignment operator.
    //
    Vector& operator= (const Vector& rhs);

    /**: Returns a modifiable lvalue reference to the i'th
               element in the Vector, counting from zero.    */
    T& operator[] (unsigned i);

    /**: Returns a constant reference to the i'th element in the Vector,
               counting from zero.  
    */
    const T& operator[] (unsigned i) const;


protected:
    //
    // The underlying vector of T representing the Vector.
    //
    T vect[N];
};

//
// Inlines.
//

template <class T, unsigned N>
inline
Vector<T,N>::Vector()
{}

template <class T, unsigned N>
inline
T&
Vector<T,N>::operator[] (unsigned i)
{
    return vect[i];
}

template <class T, unsigned N>
inline
const T&
Vector<T,N>::operator[] (unsigned i) const
{
    return vect[i];
}


template <class T, unsigned N>
Vector<T,N>::Vector (const Vector<T,N>& rhs)
{
    for (unsigned i = 0; i < N; ++i)
        vect[i] = rhs.vect[i];
}

template <class T, unsigned N>
Vector<T,N>&
Vector<T,N>::operator= (const Vector<T,N>& rhs)
{
    for (unsigned i = 0; i < N; ++i)
        vect[i] = rhs.vect[i];
    return *this;
}

/*
	combines two vecs of size N1 and N2 
	into one vec of size N1 + N2
*/
template <class T, unsigned N1, unsigned N2>
Vector<T, N1+N2> operator+ (Vector<T, N1> vec1, Vector<T, N2> vec2)
{
	Vector<T, N1 + N2> result;
	for (unsigned i = 0; i < N1; i++)
		result[i] = vec1[i];
	
	for (unsigned i = N1; i < N2; ++i)
		result[i] = vec2[i];
	
	return result;
}



}

#endif
