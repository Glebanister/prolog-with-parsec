#include <string>
using std::string;

#include <iostream>
#include "LispObject.h"

#include "Parsnip.h"

using namespace Parsnip;

double multiply(double x, double y)
{
	return x*y;
}
double add(double x, double y)
{
	return x+y;
}
double subtract(double x, double y)
{
	return x-y;
}
double divide(double x, double y)
{
	return x/y;
}


int main (int argc, char * const argv[]) 
{	
	typedef Parser<string, double>::type NumParser;

	NumParser op_self = lazy<string, double>();
	
	NumParser term =  real |  skip_ch('(') >> op_self >> skip_ch(')'); 

	NumParser ops = op_table(term)
						->infix_left("+", 10, add)
						->infix_left("-", 10, subtract)
						->infix_left("*", 20, multiply)
						->infix_left("/", 20, divide);

	setLazy(op_self, ops);

	NumParser expr =  ops | term;

	std::string input;
	ParseResult<double> result;
	

	while (true)
	{
		std::cout << "> ";
	    std::getline(std::cin, input);

		if (input == "exit") break;		
		
		result =  parse(input, expr);

		if (result.parse_finished())
		{
			cout << endl << result.data() << endl;
		}
		else if (result.input_consumed()) 
		{
			cout << "Unexpected end of input string " << endl;
		}
		else
		{
			std::string::size_type pos = result.parse_position();
			cout << "Unexpected character '" << input[pos] << "' at position " << pos  << endl;
		}
	}
	
    return 0;
}