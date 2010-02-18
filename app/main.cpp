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
#include "packageobject.hpp"
#include "classobject.hpp"
#include "operationobject.hpp"
#include "parameterobject.hpp"

using std::cout;
using std::vector;

int main(int /*argc*/, char ** /*argv*/)
{
	PackageObject * p = new PackageObject;
	ClassObject * c = new ClassObject;
	ClassObject * c2 = new ClassObject;
	OperationObject * m = new OperationObject();
	OperationObject * m1 = new OperationObject();
	OperationObject * m2 = new OperationObject();

	m->setParent(c);
	m1->setParent(c);
	m2->setParent(c);
	c->setName("Command");
	p->setName("stf");
	c2->setName("type");
	m->setName("execute");
	m1->setName("test");
	m2->setName("test2");
	m1->setReturnType(c2);

	ParameterObject * param1 = new ParameterObject;
	param1->setParent(m1);
	param1->setDatatype(c2);
	param1->setName("var1");

	c->setParent(p);
	c->setParent(p);

	std::cout << m1->qualifiedName();

	delete p;

}

