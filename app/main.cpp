/*******************************************************************
* Copyright (c) 2009 Thomas Fannes (thomasfannes@gmail.com)
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************/

#include <iostream>
#include <QSharedPointer>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "algorithm.hpp"
#include <string>
#include <boost/unordered_set.hpp>
#include <boost/lambda/lambda.hpp>
#include <memory>

using std::cout;
using std::vector;

template <unsigned int VALUE, unsigned int AMOUNT> struct Collatz
{
	enum
	{
		val = Collatz< (VALUE%2 == 0 ? VALUE/2 : VALUE*3+1), AMOUNT + 1>::val,
	};
};
template <unsigned int AMOUNT> struct Collatz<1, AMOUNT>
{
	enum
	{
		val = AMOUNT,
	};
};

int method(unsigned int value, int amount = 0)
{
	if(value == 1)
		return amount;
	else if(value%2 == 0)
		return method(value/2, amount+1);
	else
		return method(value*3+1, amount+1);

};

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << Collatz<24, 0>().val;
}

