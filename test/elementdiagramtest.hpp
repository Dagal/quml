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

#ifndef ELEMENTDIAGRAMTEST_HPP
#define ELEMENTDIAGRAMTEST_HPP

#include <QObject>
#include <QtTest/QtTest>

#include "defines.hpp"

class ElementDiagramTest : public QObject
{
	Q_OBJECT

private slots:

	void parent_AddChildOnCreation();
	void parent_AddChildAfterCreation();
	void parent_findChildren();
	void parentchild_qualifiedName();
	void parentchild_setnameQualifiedName();
	void parent_AddToDiagram();
	void parentChild_RemoveChildParent();
	void parentChild_AddParentToDiagram();
	void parentChild_AddChildToDiagram();
	void parentDiagram_AddChildToParent();
	void parentChildDiagram_RemoveChildDiagram();
	void parentChildDiagram_RemoveParentDiagram();
	void parentChildDiagram_nameChange();

	void cleanup();
	void init();

private:
	ElementObject * _parent;
	ElementObject * _child;
	UMLDiagram * _diagram;

};

#endif // ELEMENTDIAGRAMTEST_HPP
