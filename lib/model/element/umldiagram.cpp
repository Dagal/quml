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

void checkForDetaching(ElementObject * elementObject)
{
	MethodObject * method = element_cast<MethodObject>(elementObject);
	if(method && method->returnType() != 0 && method->returnType()->umlDiagram() != elementObject->umlDiagram())
			method->setReturnType(0);

	ParameterObject * param = element_cast<ParameterObject>(elementObject);
	if(param && param->datatype() != 0 && param->datatype()->umlDiagram() != elementObject->umlDiagram())
		param->setDatatype(0);
}

UMLDiagram::UMLDiagram()
	: _dd(new UMLDiagram::UMLDiagramPrivate)
{
	_dd->_diagram = this;
}

UMLDiagram::~UMLDiagram()
{
	// remove all the elements
	while(!_dd->_elements.empty())
	{
		ElementObject * curElement = _dd->_elements[0];
		curElement->setUMLDiagram(0);
		curElement->setParent(0);

		delete curElement;
	}
}

ElementObject* UMLDiagram::findElement(const std::string & qualifiedName) const
{
	std::vector<ElementObject*>::iterator i = _dd->findElement(qualifiedName);

	if(i == _dd->_elements.end())
		return 0;
	else
		return *i;
}

std::vector<ElementObject*> UMLDiagram::allElements() const
{
	return _dd->_elements;
}

std::vector<ElementObject *> UMLDiagram::findRelatedElements(ElementObject * elementObject) const
{
	return _dd->_elementRelator.findElementsRelatedTo(elementObject);
}

void UMLDiagram::UMLDiagramPrivate::attachElement(ElementObject * elementObject)
{
	// add this and children to diagram
	INT_recursiveAttachElement(elementObject);

	// check this element for "strange" links
	INT_recursiveRemoveStrangeAttachedElements(elementObject);

	// resort the list
	resortElements();
}

void UMLDiagram::UMLDiagramPrivate::detachElement(ElementObject * elementObject)
{
	// remove this and children from diagram
	INT_recursiveDetachElement(elementObject);

	// check this element for "strange" links
	INT_recursiveRemoveStrangeAttachedElements(elementObject);

	// resort the list
	resortElements();
}

void UMLDiagram::UMLDiagramPrivate::resortElements()
{
	std::sort(
			_elements.begin(),
			_elements.end(),
			boost::bind(&ElementObject::qualifiedName, _1) < boost::bind(&ElementObject::qualifiedName, _2)
			);
}

std::vector<ElementObject*>::iterator UMLDiagram::UMLDiagramPrivate::findElement(const std::string & qualifiedName)
{
	return stf::binary_find_if(
			_elements.begin(),
			_elements.end(),
			boost::bind(
					stf::comparator<std::string>(),
					boost::bind(&ElementObject::qualifiedName, _1),
					boost::cref(qualifiedName)
					)
			);
}

std::vector<ElementObject*>::iterator UMLDiagram::UMLDiagramPrivate::findElement(ElementObject * elementObject)
{
	if(elementObject == 0)
		return _elements.end();
	else
		return findElement(elementObject->qualifiedName());

}

void UMLDiagram::UMLDiagramPrivate::INT_recursiveAttachElement(ElementObject * elementObject)
{
	// set the diagram pointer
	elementObject->_dd->_diagram = _diagram;

	// add to the lists
	INT_addElementToLists(elementObject);

	// attach the children
	const std::vector<ElementObject*> & vct = elementObject->children();
	std::for_each(
			vct.begin(),
			vct.end(),
			boost::bind(&UMLDiagram::UMLDiagramPrivate::INT_recursiveAttachElement, this, _1));
}

void UMLDiagram::UMLDiagramPrivate::INT_recursiveDetachElement(ElementObject * elementObject)
{
	// detach this element
	elementObject->_dd->_diagram = 0;

	// remove from the lists
	INT_removeElementFromLists(elementObject);

	// detach the children
	const std::vector<ElementObject*> & vct = elementObject->children();
	std::for_each(
			vct.begin(),
			vct.end(),
			boost::bind(&UMLDiagram::UMLDiagramPrivate::INT_recursiveDetachElement, this, _1)
			);
}

void UMLDiagram::UMLDiagramPrivate::INT_removeElementFromLists(ElementObject * elementObject)
{
	// remove from elements
	std::vector<ElementObject*>::iterator i = std::find(
			_elements.begin(),
			_elements.end(),
			elementObject);

	if(i != _elements.end())
		_elements.erase(i);
}

void UMLDiagram::UMLDiagramPrivate::INT_addElementToLists(ElementObject * elementObject)
{
	// is there already an element with this name?
	element_vector::iterator i = findElement(elementObject->qualifiedName());
	if(i != _elements.end())
	{
		if(*i == elementObject)
			return;

		// delete the other elements
		ElementObject * toBeDeleted = *i;
		toBeDeleted->setUMLDiagram(0);
		toBeDeleted->setParent(0);

		delete toBeDeleted;
	}

	_elements.push_back(elementObject);
}

void UMLDiagram::UMLDiagramPrivate::INT_recursiveRemoveStrangeAttachedElements(ElementObject * elementObject)
{
	// check for detaching of this element
	checkForDetaching(elementObject);

	// check for detaching of attached elements
	std::vector<ElementObject*> attachedVct = _elementRelator.findElementsRelatedTo(elementObject);
	std::for_each(
			attachedVct.begin(),
			attachedVct.end(),
			boost::bind(checkForDetaching, _1)
			);

	// recursive on children
	const std::vector<ElementObject*> & vct = elementObject->children();
	std::for_each(
			vct.begin(),
			vct.end(),
			boost::bind(&UMLDiagram::UMLDiagramPrivate::INT_recursiveRemoveStrangeAttachedElements, this, _1)
			);
}
