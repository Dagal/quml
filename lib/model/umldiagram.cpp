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

#include "umldiagram.hpp"
#include "_umldiagram.hpp"
#include "elementobject.hpp"
#include <boost/bind.hpp>
#include "algorithm.hpp"

UMLDiagram::UMLDiagram()
	: _dd(new UMLDiagramPrivate(this))
{
}


UMLDiagram::UMLDiagramPrivate::elementvct::iterator UMLDiagram::UMLDiagramPrivate::findInElements(const std::string & name)
{
	return  stf::binary_find_if(
			_elements.begin(),
			_elements.end(),
			boost::bind<int>(
					ElementObject::string_comparator(),
					_1,
					boost::ref(name)
					)
			);
}

UMLDiagram::UMLDiagramPrivate::elementvct::const_iterator UMLDiagram::UMLDiagramPrivate::findInElements(const std::string & name) const
{
	return  stf::binary_find_if(
			_elements.begin(),
			_elements.end(),
			boost::bind<int>(
					ElementObject::string_comparator(),
					_1,
					boost::ref(name)
					)
			);
}

ElementObject * UMLDiagram::findElement(const std::string & qualifiedName) const
{
	UMLDiagramPrivate::elementvct::const_iterator i = _dd->findInElements(qualifiedName);
	if(i != _dd->_elements.end())
		return *i;
	else
		return 0;
}

std::vector<ElementObject *> UMLDiagram::elements() const
{
	return _dd->_elements;

}

void UMLDiagram::UMLDiagramPrivate::attachElementObject(ElementObject * element)
{
	if(!element)
		return;

	assert(element->umlDiagram() == _diagram);

	// an item already in this collection?
	emptyLocation(element->qualifiedName());

	// add the element
	_elements.push_back(element);
	resortElements();
}

void UMLDiagram::UMLDiagramPrivate::detachElementObject(const std::string & qualifiedName)
{
	_elements.erase(findInElements(qualifiedName));
}

void UMLDiagram::UMLDiagramPrivate::emptyLocation(const std::string & name)
{
	elementvct::iterator  i = findInElements(name);
	if(i != _elements.end())
		(*i)->setUMLDiagram(0);
}

void UMLDiagram::UMLDiagramPrivate::changeElementName(const std::string & /*oldName*/, const std::string & /*newName*/)
{
	// just update the complete list
	resortElements();
}

void UMLDiagram::UMLDiagramPrivate::resortElements()
{
	std::sort(_elements.begin(),
			  _elements.end(),
			  ElementObject::comparator());
}

