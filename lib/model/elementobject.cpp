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
#include "classobject.hpp"
#include "operationobject.hpp"
#include "propertyobject.hpp"
#include "classdiagram.hpp"


ElementObject::ElementObject(ElementType type, ClassDiagram * diagram)
	: _dd(new ElementObjectPrivate(type, diagram))
{

}

ElementObject::~ElementObject()
{
}

ElementType ElementObject::type() const
{
	return _dd->_type;
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

	if(oldParent) oldParent->onChildRemoved(this);
	if(parent) parent->onChildAdded(this);
}


const string & ElementObject::name() const
{
	return _dd->_name;
}

void ElementObject::setName(const string & newName)
{
	if(newName == name())
		return;

	string oldName = name();
	_dd->_name = newName;


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
	return "to be implemented";
}

ClassDiagram * ElementObject::classDiagram() const
{
	return _dd->_diagram;
}

