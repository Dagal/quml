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

#ifndef CLASSDIAGRAMRULES_HPP
#define CLASSDIAGRAMRULES_HPP

#include "iclassdiagramrules.hpp"
#include "classdiagramcontroller.hpp"

class DefaultClassDiagramRules : public IClassDiagramRules
{
public:
	DefaultClassDiagramRules(const ClassDiagramController * controller) : _controller(controller) {}

	virtual bool isValidName(const QString & newName) const;
	virtual bool isNameValidWithinParent(element::ElementObject * elementObject, element::ElementObject * parent, const QString & newName) const;

	const ClassDiagramController * controller() const { return _controller; }

private:
	const ClassDiagramController * _controller;
};


class ClassObjectDiagramRules : public DefaultClassDiagramRules
{
public:
	ClassObjectDiagramRules(const ClassDiagramController * controller) : DefaultClassDiagramRules(controller) {}

	virtual bool isParentRightContainerType(element::ElementObject * newParent) const;
};
class PackageObjectDiagramRules : public DefaultClassDiagramRules
{
public:
	PackageObjectDiagramRules(const ClassDiagramController * controller) : DefaultClassDiagramRules(controller) {}

	virtual bool isParentRightContainerType(element::ElementObject * newParent) const;
};
class MethodObjectDiagramRules : public DefaultClassDiagramRules
{
public:
	MethodObjectDiagramRules(const ClassDiagramController * controller) : DefaultClassDiagramRules(controller) {}

	virtual bool isParentRightContainerType(element::ElementObject * newParent) const;
	virtual bool isNameValidWithinParent(element::ElementObject * elementObject, element::ElementObject * parent, const QString & newName) const;
};

class OperationObjectDiagramRules : public MethodObjectDiagramRules
{
public:
	OperationObjectDiagramRules(const ClassDiagramController * controller) : MethodObjectDiagramRules(controller) {}

	virtual bool isParentRightContainerType(element::ElementObject * newParent) const;
};

class PropertyObjectDiagramRules : public DefaultClassDiagramRules
{
public:
	PropertyObjectDiagramRules(const ClassDiagramController * controller) : DefaultClassDiagramRules(controller) {}

	virtual bool isParentRightContainerType(element::ElementObject * newParent) const;
};



#endif // CLASSDIAGRAMRULES_HPP
