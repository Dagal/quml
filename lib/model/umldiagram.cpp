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

UMLDiagram::UMLDiagram()
	: _dd(new UMLDiagramPrivate(this))
{
}

ElementObject * UMLDiagram::findElement(const std::string & qualifiedName) const
{
	UMLDiagramPrivate::elementmap::iterator i = _dd->_elements.find(qualifiedName);
	if(i != _dd->_elements.end())
		return i->second;
	else
		return 0;
}

std::vector<ElementObject *> UMLDiagram::elements() const
{
	typedef std::vector<ElementObject*> elementvector;
	elementvector vct;

	std::for_each(
			_dd->_elements.begin(),
			_dd->_elements.end(),

			boost::bind(
					// insert into the vector
					&elementvector::push_back,
					boost::ref(vct),
					boost::bind(
							// the value of the unordered_map
							&UMLDiagramPrivate::elementmap::value_type::second,
							_1)
					)
			);

	return vct;

}

void UMLDiagram::UMLDiagramPrivate::attachElementObject(ElementObject * element)
{
	if(!element)
		return;

	assert(element->umlDiagram() == _diagram);

	// an item already in this collection?
	elementmap::iterator i = _elements.find(element->qualifiedName());
	if(i != _elements.end())
		i->second->setUMLDiagram(0);

	// add the element
	_elements[element->qualifiedName()] = element;
}

void UMLDiagram::UMLDiagramPrivate::detachElementObject(const std::string & qualifiedName)
{
	_elements.erase(qualifiedName);
}
