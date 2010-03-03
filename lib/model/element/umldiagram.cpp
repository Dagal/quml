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
#include "elementhelper.hpp"
#include "_elementobject.hpp"
#include "algorithm.hpp"
#include "methodobject.hpp"
#include "parameterobject.hpp"

bool elementComparator(boost::shared_ptr< ElementObject> elementA, boost::shared_ptr<ElementObject> elementB)
{
	return elementA->qualifiedName() < elementB->qualifiedName();
}

int elementFinder(boost::shared_ptr<ElementObject> element, const std::string & qualifiedName)
{
	return element->qualifiedName().compare(qualifiedName);
}

std::string elementToRelatedQualifiedName(ElementObject * element)
{
	if(element == 0)
		return std::string();

	// is it a method?
	MethodObject * method = element_cast<MethodObject>(element);
	if(method) return method->returnType();

	// is it a parameter?
	ParameterObject * param = element_cast<ParameterObject>(element);
	if(param) return param->datatype();

	return std::string();
}

UMLDiagram::UMLDiagram()
		: _dd(new UMLDiagram::UMLDiagramPrivate)
{
	_dd->_diagram = this;
}

void UMLDiagram::attachElement(ElementObject * elementObject)
{
	// is this element existing?
	if(elementObject == 0)
		return;

	// attached to this diagram?
	if(elementObject->umlDiagram() == this)
		return;

	// attached to another diagram?
	if(elementObject->umlDiagram() != 0)
		elementObject->umlDiagram()->detachElement(elementObject);

	// make sure it is not attached to a parent
	elementObject->setParent(0);

	_dd->INT_attachElement(elementObject);
}

void UMLDiagram::detachElement(ElementObject * elementObject)
{
	// is this element existing?
	if(elementObject == 0)
		return;

	_dd->INT_detachElement(elementObject);
}

std::vector<ElementObject*> UMLDiagram::findElements(const std::string & qualifiedName) const
{
	// get the memory
	std::vector<ElementObject*> foundElements(_dd->_elements.size());

	// copy if the name matches
	std::vector<ElementObject*>::iterator i = stf::copy_if(
			_dd->_elements.begin(),
			_dd->_elements.end(),
			foundElements.begin(),
			boost::bind(
					std::equal_to<const std::string &>(),
					boost::cref(qualifiedName),
					boost::bind(&ElementObject::qualifiedName, _1)
					)
			);

	foundElements.erase(i, foundElements.end());

	return foundElements;
}

std::vector<ElementObject*> UMLDiagram::allElements() const
{
	return _dd->_elements;
}

std::vector<ElementObject *> UMLDiagram::findRelatedElements(ElementObject * elementObject) const
{
	UMLDiagramPrivate::relatedElements_map::iterator i = _dd->_relatedElements.find(elementObject);
	if(i == _dd->_relatedElements.end())
			return std::vector<ElementObject *>();
	else
		return *(i->second);
}

void UMLDiagram::UMLDiagramPrivate::INT_attachElement(ElementObject * elementObject)
{
	// set the diagram
	elementObject->_dd->_diagram = _diagram;

	// attach this object
	_elements.push_back(elementObject);

	// attach all the children
	const element_vector & children = elementObject->children();
	std::for_each(
			children.begin(),
			children.end(),
			boost::bind(&UMLDiagram::UMLDiagramPrivate::INT_attachElement, this, _1)
			);
}

void UMLDiagram::UMLDiagramPrivate::INT_detachElement(ElementObject * elementObject)
{
	// detach this object
	element_vector::iterator i = std::find(_elements.begin(), _elements.end(), elementObject);
	if(i == _elements.end())
		return;

	_elements.erase(i);

	// remove the diagram
	elementObject->_dd->_diagram = 0;

	// detach all the children
	const element_vector & children = elementObject->children();
	std::for_each(
			children.begin(),
			children.end(),
			boost::bind(&UMLDiagram::UMLDiagramPrivate::INT_attachElement, this, _1)
			);
}
