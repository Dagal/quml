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
#include "packageobject.hpp"
#include "umldiagram.hpp"
#include <iostream>
#include <algorithm>

void ElementDiagramTest::init()
{
	_diagram = new UMLDiagram;
	_parent = new PackageObject;
	_parent->setName("parent");
	_child = new PackageObject;
	_child->setName("child");
}

void ElementDiagramTest::cleanup()
{
	delete _child;
	delete _parent;
	delete _diagram;
}

void ElementDiagramTest::parent_AddChildOnCreation()
{
	delete _child;
	_child = new PackageObject(_parent);

	QCOMPARE(_parent->children().size(), (unsigned int)1);
	QCOMPARE(_parent->children()[0], _child);
	QCOMPARE(_child->parent(), _parent);
}

void ElementDiagramTest::parent_AddChildAfterCreation()
{
	_child->setParent(_parent);

	QCOMPARE(_parent->children().size(), (unsigned int)1);
	QCOMPARE(_parent->children()[0], _child);
	QCOMPARE(_child->parent(), _parent);
}

void ElementDiagramTest::parentchild_qualifiedName()
{
	_child->setParent(_parent);

	QCOMPARE(_parent->qualifiedName(), std::string("parent"));
	QCOMPARE(_child->qualifiedName(), std::string("parent::child"));
}

void ElementDiagramTest::parentchild_setnameQualifiedName()
{
	_child->setParent(_parent);
	_parent->setName("parent_new");
	_child->setName("child_new");

	QCOMPARE(_parent->name(), std::string("parent_new"));
	QCOMPARE(_child->name(), std::string("child_new"));
	QCOMPARE(_child->qualifiedName(), std::string("parent_new::child_new"));
}

void ElementDiagramTest::parent_AddToDiagram()
{
	_parent->setUMLDiagram(_diagram);

	QCOMPARE(_diagram->elements().size(), (unsigned int)1);
	QCOMPARE(_diagram->elements()[0], _parent);
	QCOMPARE(_diagram->findElement(_parent->qualifiedName()), _parent);
}

void ElementDiagramTest::parentChild_RemoveChildParent()
{
	_child->setParent(_parent);

	delete _child;
	_child = 0;

	QCOMPARE(_parent->children().size(), (unsigned int)0);
}

void ElementDiagramTest::parentChild_AddParentToDiagram()
{
	_child->setParent(_parent);
	_parent->setUMLDiagram(_diagram);

	QCOMPARE(_parent->umlDiagram(), _diagram);
	QCOMPARE(_child->umlDiagram(), _diagram);
	QCOMPARE(_diagram->elements().size(), (unsigned int)2);
}

void ElementDiagramTest::parentChild_AddChildToDiagram()
{
	_child->setParent(_parent);
	_child->setUMLDiagram(_diagram);

	QVERIFY(_parent->umlDiagram() == 0);
	QCOMPARE(_child->umlDiagram(), _diagram);
	QVERIFY(_child->parent() == 0);

}

void ElementDiagramTest::parentDiagram_AddChildToParent()
{
	_parent->setUMLDiagram(_diagram);
	_child->setParent(_parent);

	QCOMPARE(_child->umlDiagram(), _diagram);
}

void ElementDiagramTest::parentChildDiagram_RemoveChildDiagram()
{
	_parent->setUMLDiagram(_diagram);
	_child->setParent(_parent);

	_child->setUMLDiagram(0);
	QVERIFY(_child->umlDiagram() == 0);
	QVERIFY(_child->parent() == 0);
	QCOMPARE(_parent->umlDiagram(), _diagram);
}

void ElementDiagramTest::parentChildDiagram_RemoveParentDiagram()
{
	_parent->setUMLDiagram(_diagram);
	_child->setParent(_parent);

	_parent->setUMLDiagram(0);
	QVERIFY(_child->umlDiagram() == 0);
	QVERIFY(_child->parent() == _parent);
	QVERIFY(_parent->umlDiagram() == 0);
}

void ElementDiagramTest::parentChildDiagram_nameChange()
{
	_child->setParent(_parent);
	_parent->setUMLDiagram(_diagram);
	_parent->setName("parent_new");

	QCOMPARE(_diagram->findElement(_child->qualifiedName()), _child);
	QCOMPARE(_diagram->findElement(_parent->qualifiedName()), _parent);
}
