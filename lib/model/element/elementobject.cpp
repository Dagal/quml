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
	: _dd(new ElementObjectPrivate(this))
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

void ElementObject::setUMLDiagram(UMLDiagram * diagram)
{
	// stupid function call?
	if(umlDiagram() == diagram)
		return;

	// detach from parent if possible
	if(parent() != 0)
		_dd->detachFromParent();

	// detach from umlDiagram if possible
	if(umlDiagram() != 0)
		_dd->detachFromUML();

	if(diagram == 0)
		return;

	diagram->_dd->attachElement(this);
}

void ElementObject::setName(const std::string & name)
{
	// stupid function call?
	if(name == _dd->_name)
		return;

	// attached to umlDiagram?
	if(umlDiagram())
	{
		// find the "new" qualified name
		std::string oldName = _dd->_name;
		_dd->_name  = name;
		std::string qualName = qualifiedName();
		_dd->_name = oldName;

		// name already taken?
		ElementObject * rivalElement = umlDiagram()->findElement(qualName);
		if(rivalElement != 0)
		{
			rivalElement->setUMLDiagram(0);
			rivalElement->setParent(0);

			delete rivalElement;
		}
	}

	_dd->_name = name;
	if(umlDiagram())
		umlDiagram()->_dd->resortElements();
}

void ElementObject::setParent(ElementObject * parent)
{
	// same parent?
	if(_dd->_parent == parent)
		return;

	/* **********************************************
	   Find which case we are dealing with:
	   1) simply detach from parent
	   2) attach to new parent
	   3) 1 & 2 together
	   4) attach to new parent in different umlDiagram
	   5) detach from umlDiagram, from parent and attach to new parent in different umlDiagram
	   6) detach from umlDiagram, from parent and attach to new parent (with no umlDiagram)
	   */

	if(parent == 0)
	{
		// (1)
		_dd->detachFromParent();
	}
	else
	{
		// uml diagram the same?
		if(parent->umlDiagram() == umlDiagram())
		{
			// (2) or (3)
			if(_dd->_parent != 0)
				_dd->detachFromParent();

			_dd->attachToParentInSameUML(parent);
		}
		else
		{
			// (4), (5) or (6)
			if(umlDiagram() != 0)
				_dd->detachFromUML();

			if(parent->umlDiagram() == 0)
				// (6)
				_dd->attachToParentInSameUML(parent);
			else
				// (5)
				_dd->attachToParentInDifferentUML(parent);
		}
	}
}

void ElementObject::ElementObjectPrivate::detachFromParent()
{
	assert(_parent != 0);

	// remove the parent
	ElementObject * oldParent = _parent;
	_parent = 0;

	// remove from the parents children
	oldParent->_dd->INT_removeChild(_element);

	// send the message to the parent
	oldParent->onChildRemoved(_element);
}

void ElementObject::ElementObjectPrivate::attachToParentInSameUML(ElementObject * newParent)
{
	assert(newParent != 0 && _parent == 0);

	// set the new parent
	_parent = newParent;

	// add to the parents children
	_parent->_dd->INT_addChild(_element);

	// send the message
	_parent->onChildAdded(_element);
}

void ElementObject::ElementObjectPrivate::attachToParentInDifferentUML(ElementObject * parent)
{
	assert(parent != 0 && parent->umlDiagram() != 0 && _parent == 0 && _diagram == 0);

	// set the new parent & diagram
	_parent = parent;
	_diagram = parent->umlDiagram();

	// add to the parent
	_parent->_dd->INT_addChild(_element);

	// add to the umlDiagram
	_diagram->_dd->attachElement(_element);

	// send the message
	_parent->onChildAdded(_element);
}

void ElementObject::ElementObjectPrivate::detachFromUML()
{
	assert(_diagram != 0);

	// detach the diagram
	_diagram->_dd->detachElement(_element);
}

void ElementObject::ElementObjectPrivate::INT_removeChild(ElementObject * child)
{
	std::vector<ElementObject*>::iterator i = std::find( _children.begin(), _children.end(), child);

	if(i != _children.end())
		_children.erase(i);
}

void ElementObject::ElementObjectPrivate::INT_addChild(ElementObject * child)
{
	_children.push_back(child);
}

void ElementObject::relatedElementChanged(ElementObject * oldRelatedElement)
{
	if(umlDiagram())
		umlDiagram()->_dd->_elementRelator.updateElementObject(this, oldRelatedElement);
}

