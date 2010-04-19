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

#ifndef ELEMENTRELATORTEST_HPP
#define ELEMENTRELATORTEST_HPP

#include <QObject>
#include <QtTest/QtTest>

#include "element.hpp"

class ElementRelatorTest : public QObject
{
	Q_OBJECT

public:
    ElementRelatorTest();


private slots:
	void cleanup();
	void init();

	void relatorTest();

	void propertyDatatypeDetachFromUML();
	void propertyDetachFromUML();
	void propertyParentDetachFromUML();
	void propertyParentAndDatatypeParentDetachFromUML();
	void propertyDatatypeAttachToNewUML();
	void propertyAttachToNewUML();
	void propertyParentAttachToNewUML();
	void propertyParentAndDatatypeParentAttachToNewUML();

private:
	element::ClassObject * data1;
	element::ClassObject * data2;
	element::PackageObject* pack1;
	element::PropertyObject * prop1;
	element::UMLDiagram * diagram;
	element::UMLDiagram * newdiagram;
};

#endif // ELEMENTRELATORTEST_HPP
