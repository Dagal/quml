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

#include "classdiagram.hpp"
#include "_classdiagram.hpp"
#include "elementobject.hpp"
#include "_elementobject.hpp"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/get_pointer.hpp>
#include <map>
#include <iostream>

using namespace std;

ClassDiagram::ClassDiagram()
	: _dd(new ClassDiagramPrivate)
{
}

void ClassDiagram::attachElementObject(ElementObjectPtr elementObject)
{
	if(!elementObject)
		return;

	if(elementObject->classDiagram() == this)
		return;

	// is there a free space to attacht this element?
	ElementObject * otherObject = findElementsByQualifiedName(elementObject->qualifiedName());
	if(otherObject != 0)
		// other object in place
		detachElementObject(otherObject->qualifiedName());

	// insert the element
	_dd->_elements[elementObject->qualifiedName()] = elementObject;

	// release the old classdiagram and set the new
	if(elementObject->classDiagram())
		elementObject->classDiagram()->detachElementObject(elementObject->qualifiedName());

	elementObject->_dd->_diagram = this;

}

ElementObjectPtr ClassDiagram::detachElementObject(const string & elementName)
{
	elementmap::const_iterator it = _dd->_elements.find(elementName);

	// is this element present?
	if(it == _dd->_elements.end())
		return ElementObjectPtr();

	// detach it
	ElementObjectPtr val  = it->second;
	_dd->_elements.erase(it);

	// and release us from it
	val->_dd->_diagram = 0;

	return val;
}

ElementObject * ClassDiagram::findElementsByQualifiedName(const string & elementName)
{
	elementmap::const_iterator it = _dd->_elements.find(elementName);

	if(it == _dd->_elements.end())
		return 0;
	else
		return it->second.get();
}

void ClassDiagram::onElementNameChanged(ElementObject * element, const std::string & oldName)
{
	_dd->renameElement(element, oldName);


}

void ClassDiagram::ClassDiagramPrivate::renameElement(ElementObject * element, const std::string & oldName)
{
	// find the element
	elementmap::iterator it = _elements.find(oldName);
	assert(it != _elements.end() && it->second.get() == element);
	ElementObjectPtr ptr = it->second;

	// erase the old one
	_elements.erase(it);

	// insert the new one
	_elements[element->qualifiedName()] = ptr;
}

void print_string(ElementNameRelatorPtr ptr) {
  std::cout << ptr.get() << '\n';
}

void ClassDiagram::ClassDiagramPrivate::removeFromRelators(ElementObject * element)
{
	// remove all relators where this is the key
	relatormap::iterator it = _relators.find(element);
	if(it != _relators.end())
		_relators.erase(it);

	// remove line from relators where this is the target
	std::for_each(
			 _relators.begin(),
			 _relators.end(),
			 boost::bind(
					 &ElementNameRelator::removeAlterer,
					 boost::bind(&relatormap::value_type::second, _1),
					 element));
}
