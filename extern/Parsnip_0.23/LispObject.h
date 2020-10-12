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

#ifndef PARSNIP_LISP_OBJECT_H
#define PARSNIP_LISP_OBJECT_H


#include <string>
using std::string;

#include "ParsnipBase.h"
#include "Accumulators.h"

namespace Parsnip
{

struct LispObject
{
	virtual bool isCons() { return false; }
	virtual bool isNumber() { return false; }
	virtual bool isSymbol() { return false; }
	virtual bool isNil() { return false; }

	virtual std::string toString()=0;
};

typedef ptr<LispObject> ObjPtr; 

struct Cons : public LispObject
{
	Cons(ObjPtr _head, ObjPtr _tail) : head(_head), tail(_tail) {}

	virtual bool isCons() { return true; }

	ObjPtr head;
	ObjPtr tail;

	virtual std::string toString()
	{
		std::string str = "(";
		str += head->toString();
		str += " . ";
		str += tail->toString();
		str += ")";
		return str;
	}
};

typedef ptr<Cons> ConsPtr;



ObjPtr makeCons(ObjPtr head, ObjPtr tail)
{
	return new Cons(head, tail);
}

Cons* toCons(ObjPtr obj)
{
	return static_cast<Cons*>(obj.GetRawPointer());
}

struct Symbol : public LispObject
{
	Symbol(const std::string& _name) : name(_name) {}
	virtual bool isSymbol() { return true; }
	std::string name;
	std::string toString() { return name; }
};

typedef ptr<Symbol> SymPtr;

ObjPtr makeSymbol(const std::string& str)
{
	return new Symbol(str);
}

struct Number : public LispObject 
{
	Number(double _val) : value(_val) {}
	virtual bool isNumber() { return true; }
	double value; 

	std::string toString() { return to_string(value); }
};

typedef ptr<Number> NumPtr;

ObjPtr makeNumber(double val)
{
	return new Number(val);
}

struct NilObject : public LispObject
{
	bool isNil() { return true; }
	
	static ObjPtr getNil()
	{
		if (nil) { return nil; }
		else 
		{
			nil = new NilObject;
			return nil;
		}
	}

	std::string toString() { return "()"; }
private:
	static ObjPtr nil;
};

ObjPtr NilObject::nil;

ObjPtr getNil()
{
	return NilObject::getNil();
}



struct BuildCons : public Accumulator<ObjPtr, ObjPtr >
{
	BuildCons()
	{
		first_cell = getNil();
		last_cell = first_cell;
	}

	virtual void accum(const ObjPtr& o) 
	{ 
		static ObjPtr nil = getNil();

		if(first_cell->isCons())
		{
			ObjPtr new_cell = makeCons(o, nil);
			toCons(last_cell)->tail = new_cell;
			last_cell = new_cell;
		}
		else
		{
			first_cell = makeCons(o, nil);
			last_cell = first_cell;
		}
	}

	virtual ObjPtr result() { return first_cell; }

private:
	ObjPtr first_cell;
	ObjPtr last_cell;
};


}
#endif