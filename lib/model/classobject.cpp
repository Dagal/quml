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

#include "classobject.hpp"
#include "propertyobject.hpp"
#include "operationobject.hpp"
#include "_classobject.hpp"
#include <iostream>


template <typename T> bool tryToAddToVector(ElementObject * child, std::vector<T*> & vct)
{
	if(!child)
		return false;

	T * element = element_cast<T>(child);
	if(!element)
		return false;

	vct.push_back(element);
	return true;
}

template <typename T> bool tryToRemoveFromVector(ElementObject * child, std::vector<T*> & vct)
{
	if(!child)
		return false;

	T * element = element_cast<T>(child);
	if(!element)
		return false;

	vct.erase(std::find(vct.begin(), vct.end(), element));
	return true;
}

ClassObject::ClassObject(ElementObject * parent)
	: DatatypeObject(parent), _dd(new ClassObjectPrivate)
{
}

void ClassObject::onChildAdded(ElementObject * child)
{
	if (tryToAddToVector(child, _dd->_operations)) return;
	if (tryToAddToVector(child, _dd->_properties)) return;
}

void ClassObject::onChildRemoved(ElementObject * child)
{
	if (tryToRemoveFromVector(child, _dd->_operations)) return;
	if (tryToRemoveFromVector(child, _dd->_properties)) return;
}

const std::vector<OperationObject*> & ClassObject::operations() const
{
	return _dd->_operations;
}
const std::vector<PropertyObject*> & ClassObject::properties() const
{
	return _dd->_properties;
}
