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

bool ElementObject::comparator::operator()(const ElementObject * elementA, const ElementObject * elementB) const
{
	return elementA->qualifiedName() < elementB->qualifiedName();
}

int ElementObject::string_comparator::operator()(const ElementObject * element, const std::string & name) const
{
	return element->qualifiedName().compare(name);
}


ElementObject::ElementObject()
	: _dd(new ElementObjectPrivate)
{
}

ElementObject::~ElementObject()
{
	setUMLDiagram(0);
	setParent(0);

	std::vector<ElementObject*> childrenCopy = children();

	std::for_each(
			childrenCopy.begin(),
			childrenCopy.end(),
			boost::bind<void>(&deleter<ElementObject>, _1)
			);
}

void ElementObject::ElementObjectPrivate::removeChild(ElementObject * child)
{
	_children.erase(
			std::remove(
					_children.begin(),
					_children.end(),
					child));

}

void ElementObject::setParent(ElementObject * parent)
{
	if(_dd->_parent == parent)
		return;

	ElementObject * oldParent = _dd->_parent;
	_dd->_parent = parent;

	if(oldParent) oldParent->_dd->removeChild(this);
	if(parent) parent->_dd->_children.push_back(this);

	if(parent) setUMLDiagram(parent->umlDiagram());

	if(oldParent) oldParent->onChildRemoved(this);
	if(parent) parent->onChildAdded(this);
}


const string & ElementObject::name() const
{
	return _dd->_name;
}

std::string ElementObject::umlName() const
{
	return name();
}

void ElementObject::setName(const string & newName)
{
	if(newName == name())
		return;

	string oldName = name();
	_dd->_name = newName;

	if(umlDiagram()) umlDiagram()->_dd->changeElementName(oldName, newName);
	if(parent()) parent()->onChildNameChanged(oldName, this);
}

void ElementObject::onChildAdded(ElementObject * /*child*/)
{
}

void ElementObject::onChildRemoved(ElementObject * /*child*/)
{
}

void ElementObject::onChildNameChanged(const std::string & /*oldName*/, ElementObject * /*child*/)
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

void ElementObject::setUMLDiagram(UMLDiagram * diagram)
{
	// is it useful to do this?
	if (umlDiagram() == diagram)
		return;

	UMLDiagram * oldOne = umlDiagram();


	// should we detach from parent?
	if(parent() && parent()->umlDiagram() != diagram)
		setParent(0);

	// set the new diagram
	_dd->_diagram = diagram;

	// remove from old Diagram
	if(oldOne) oldOne->_dd->detachElementObject(qualifiedName());

	// add to new diagram
	if(diagram) diagram->_dd->attachElementObject(this);

	// update the children
	std::for_each(
			_dd->_children.begin(),
			_dd->_children.end(),
			boost::bind(
					&ElementObject::setUMLDiagram,
					_1,
					diagram)
			);
}

const std::vector<ElementObject *> & ElementObject::children() const
{
	return _dd->_children;
}

