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


template <typename T> struct comparator
{
	typedef T result_type;

	int operator()(const T & a, const T & b) const
	{
		if(a < b)		return -1;
		else if(b < a)	return 1;
		else			return 0;
	}
};

struct A
{
	A(int i) : j(i) {}

	int getJ() const { return j; }

	int j;
};

bool testje(const A & a, int b)
{
	return a.getJ() < b;
}

int main(int /*argc*/, char ** /*argv*/)
{
	std::vector<A> vct;
	vct.push_back(A(7));
	vct.push_back(A(1));
	vct.push_back(A(3));
	vct.push_back(A(6));
	vct.push_back(A(4));

	std::sort(
			vct.begin(),
			vct.end(),
			boost::bind(&A::getJ, _1) < boost::bind(&A::getJ, _2)
			);

	std::vector<A>::iterator i = stf::binary_find_if(
			vct.begin(),
			vct.end(),
			boost::bind(
					stf::comparator<int>(),
					boost::bind(&A::getJ, _1),
					5
					)
			);
}

