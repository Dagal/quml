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

#include "elementobject.hpp"
#include "_elementobject.hpp"
#include "umldiagram.hpp"
#include "_umldiagram.hpp"
#include <boost/bind.hpp>

template<typename T> void deleter(T * element)
{
	delete element;
}
ElementObject::ElementObject(const std::string & name)
	: _dd(new ElementObjectPrivate)
{
	_dd->_name = name;
}

ElementObject::~ElementObject()
{
	if(umlDiagram() != 0)
		return;

	std::vector<ElementObject*> childrenCopy = children();

	std::for_each(
			childrenCopy.begin(),
			childrenCopy.end(),
			boost::bind<void>(&deleter<ElementObject>, _1)
			);
}

const string & ElementObject::name() const
{
	return _dd->_name;
}

std::string ElementObject::umlName() const
{
	return name();
}

void ElementObject::onChildAdded(ElementObject * /*child*/)
{
}

void ElementObject::onChildRemoved(ElementObject * /*child*/)
{
}

void ElementObject::onRelatedElementChanged(const std::string & /*oldRelatedElementQualifiedName*/, const std::string & /*newRelatedElementQualifiedName*/)
{

}

ElementObject * ElementObject::parent() const
{
	return _dd->_parent;
}

string ElementObject::qualifiedName() const
{
	if(parent())
		return parent()->qualifiedName() + "::" + umlName();
	else
		return umlName();
}

UMLDiagram * ElementObject::umlDiagram() const
{
	return _dd->_diagram;
}

const std::vector<ElementObject *> & ElementObject::children() const
{
	return _dd->_children;
}

void ElementObject::relatedElementChanged(const std::string & oldRelatedElementQualifiedName, const std::string & newRelatedElementQualifiedName)
{
	if(umlDiagram())
		umlDiagram()->_dd->updateRelatedValueElement(this, oldRelatedElementQualifiedName, newRelatedElementQualifiedName);
}

void ElementObject::ElementObjectPrivate::removeChild(ElementObject * child)
{
	std::vector<ElementObject*>::iterator i = std::find(
			_children.begin(),
			_children.end(),
			child);

	if(i != _children.end())
		_children.erase(i);
}

void ElementObject::ElementObjectPrivate::addChild(ElementObject * child)
{
	if(child)
		_children.push_back(child);
}
