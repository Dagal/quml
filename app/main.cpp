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

using std::cout;
using std::vector;




struct test
{
	struct comparator
	{
		bool operator()(const test & t1, const test & t2) const
		{
			return t1.name() < t2.name();
		}
	};

	test(std::string name)
		: _name(name) {}

	const std::string & name() const
	{
		return _name;
	}

	void print()
	{
		std::cout << _name;
	}

	std::string _name;
};

int main(int argc, char ** argv)
{
	std::vector<test> v;
	v.push_back(test("een"));
	v.push_back(test("twee"));
	v.push_back(test("drie"));
	v.push_back(test("aaaa"));

	std::sort(
			v.begin(),
			v.end(),
			test::comparator()
			);
	std::string a = "twee";

	std::vector<test>::iterator i = stf::fan_find(
			v.begin(),\
			v.end(),
			boost::bind(
					static_cast<int (std::string::*)(const std::string &) const> (&std::string::compare),
					boost::bind(&test::name, _1),
					a
					)
			);

	if(i != v.end())
		std::cout << i->name();
	else
		std::cout << "dijn";
}
