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

#include "elementrelatortest.hpp"
#include "classobject.hpp"
#include "umldiagram.hpp"
#include "propertyobject.hpp"
#include "packageobject.hpp"
#include "operationobject.hpp"

ElementRelatorTest::ElementRelatorTest()
{
}

void ElementRelatorTest::cleanup()
{
	delete diagram;
	delete newdiagram;
}

void ElementRelatorTest::relatorTest()
{
	QCOMPARE(diagram->findRelatedElements(data2).size(), 2);
	QCOMPARE(diagram->findRelatedElements(data1).size(), 1);
}

void ElementRelatorTest::propertyDatatypeDetachFromUML()
{
	data2->setUMLDiagram(0);

	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);

	delete data2;
}

void ElementRelatorTest::propertyDetachFromUML()
{
	prop1->setUMLDiagram(0);

	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);
	QCOMPARE(prop1->umlDiagram(), (element::UMLDiagram*)0);

	delete prop1;
}

void ElementRelatorTest::propertyParentDetachFromUML()
{
	data1->setUMLDiagram(0);

	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);

	delete data1;
}

void ElementRelatorTest::propertyParentAndDatatypeParentDetachFromUML()
{
	pack1->setUMLDiagram(0);

	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)data2);

	delete pack1;
}

void ElementRelatorTest::propertyDatatypeAttachToNewUML()
{
	data2->setUMLDiagram(newdiagram);
	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);
}

void ElementRelatorTest::propertyAttachToNewUML()
{
	prop1->setUMLDiagram(newdiagram);
	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);
}

void ElementRelatorTest::propertyParentAttachToNewUML()
{
	data1->setUMLDiagram(newdiagram);
	QCOMPARE(prop1->datatype(), (element::DatatypeObject*)0);
}

void ElementRelatorTest::propertyParentAndDatatypeParentAttachToNewUML()
{
	pack1->setUMLDiagram(newdiagram);
	QCOMPARE(prop1->datatype(), data2);
}

void ElementRelatorTest::init()
{
	diagram = new element::UMLDiagram();
	newdiagram = new element::UMLDiagram();
	data1 = new element::ClassObject("data1");
	data2 = new element::ClassObject("data2");
	prop1 = new element::PropertyObject("prop1");
	pack1 = new element::PackageObject("package");
	oper1 = new element::OperationObject("oper1");

	data1->setParent(pack1);
	prop1->setParent(data1);
	oper1->setParent(data1);
	data2->setParent(pack1);
	pack1->setUMLDiagram(diagram);

	prop1->setDatatype(data2);

	oper1->addParameter(element::Parameter("param1", data1));
	oper1->addParameter(element::Parameter("param2", data2));
}
