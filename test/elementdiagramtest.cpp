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

#include "elementdiagramtest.hpp"
#include "elementhelper.hpp"
#include "packageobject.hpp"
#include "umldiagram.hpp"
#include "classobject.hpp"
#include <iostream>
#include <algorithm>
#include "propertyobject.hpp"

using namespace element;

ElementDiagramTest::ElementDiagramTest()
{
}

void ElementDiagramTest::init()
{
	_diagram = new UMLDiagram;
	_parent = new PackageObject("parent");
	_child = new ClassObject("child");

	_parent->setUMLDiagram(_diagram);
	_child->setParent(_parent);
}

void ElementDiagramTest::cleanup()
{
	delete _diagram;
}

void ElementDiagramTest::checkBasics()
{
	QCOMPARE(_diagram->upperLevelElements().size(), 1);
	QVERIFY(_parent->qualifiedName() == "parent");
	QVERIFY(_child->qualifiedName() == "parent::child");
	QCOMPARE(_diagram->findElement(_parent->qualifiedName()), _parent);
	QCOMPARE(_diagram->findElement(_child->qualifiedName()), _child);
	QCOMPARE(_parent->children().size(), 1);
	QCOMPARE(_parent->children()[0], _child);
	QCOMPARE(_diagram->findElements<ClassObject>().size(), 1);
}

void ElementDiagramTest::removeChild()
{
	_child->setUMLDiagram(0);

	QCOMPARE(_diagram->upperLevelElements().size(), 1);
	QCOMPARE(_diagram->upperLevelElements()[0], _parent);
	QCOMPARE(_child->umlDiagram(), (UMLDiagram*)0);
	QCOMPARE(_child->parent(), (ElementObject*)0);
	QCOMPARE(_parent->children().size(), 0);
}


void ElementDiagramTest::removeParent()
{

}
