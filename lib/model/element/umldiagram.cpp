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

bool elementComparator(boost::shared_ptr<element::ElementObject> elementA, boost::shared_ptr<element::ElementObject> elementB)
{
	return elementA->qualifiedName() < elementB->qualifiedName();
}

int elementFinder(boost::shared_ptr<element::ElementObject> element, const std::string & qualifiedName)
{
	return element->qualifiedName().compare(qualifiedName);
}

void checkForDetaching(element::ElementObject * elementObject)
{
	using namespace element;

	MethodObject * method = element_cast<MethodObject>(elementObject);
	if(method && method->returnType() != 0 && method->returnType()->umlDiagram() != elementObject->umlDiagram())
		method->setReturnType(0);

	ParameterObject * param = element_cast<ParameterObject>(elementObject);
	if(param && param->datatype() != 0 && param->datatype()->umlDiagram() != elementObject->umlDiagram())
		param->setDatatype(0);
}

namespace element
{

	/*!
	  This constructor creates an empty UMLDiagram
	*/
	UMLDiagram::UMLDiagram()
		: _dd(new UMLDiagram::UMLDiagramPrivate)
	{
		_dd->_diagram = this;
	}

	/*!
	  This destructor destroys the UMLDiagram, but also all the attached elements
	*/
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

	/*!
	  This function creates an ElementObject of type \c type, set's the name and the parent and adds it to the UMLDiagram.
	  if the parent is not found, the element has no parent.
	  This functions returns a pointer to the newly created element.
	*/
	ElementObject * UMLDiagram::createElement(ElementType type, const std::string & name, const std::string & parentQualifiedName)
	{
		if(name.empty())
			return 0;

		ElementObject * element = createElementObject(type, name);
		ElementObject * parent = findElement(parentQualifiedName);

		element->setParent(parent);
		element->setUMLDiagram(this);

		return element;
	}

	/*!
	  This function searches the UMLDiagram for an element with the specified qualifiedName, and returns the element. If no element was found
	  the function returns zero.

	  \sa element::ElementObject::qualifiedName
	*/
	ElementObject* UMLDiagram::findElement(const std::string & qualifiedName) const
	{
		std::vector<ElementObject*>::iterator i = _dd->findElement(qualifiedName);

		if(i == _dd->_elements.end())
			return 0;
		else
			return *i;
	}

	/*!
	  This function returns a list of all the attached ElementObject's
	*/
	const std::vector<ElementObject*> &  UMLDiagram::allElements() const
	{
		return _dd->_elements;
	}

	/*!
	  This function return all the elements that depend on \c elementObject. For example, for a certain datatype it will return all the methods that
	  have this datatype as returntype of all the parameters that have this as a datatype.
	*/
	std::vector<ElementObject *> UMLDiagram::findRelatedElements(ElementObject * elementObject) const
	{
		return _dd->_elementRelator.findElementsRelatedTo(elementObject);
	}

	/*!
	  \internal
	  This function is called from ElementObject to attach \c elementObject to this diagram
	*/
	void UMLDiagram::UMLDiagramPrivate::attachElement(ElementObject * elementObject)
	{
		// add this and children to diagram
		INT_recursiveAttachElement(elementObject);

		// check this element for "strange" links
		INT_recursiveRemoveStrangeAttachedElements(elementObject);

		// resort the list
		resortElements();
	}

	/*!
	  \internal
	  This function is called from ElementObject to detach \c elementObject from this diagram. The element and all its children are removed from the list
	  and afterwards all the related links are checked. This way links between children of \c elementObject are saved where links from children of \c elementObject
	  to external elements are removed.
	*/
	void UMLDiagram::UMLDiagramPrivate::detachElement(ElementObject * elementObject)
	{
		// remove this and children from diagram
		INT_recursiveDetachElement(elementObject);

		// check this element for "strange" links
		INT_recursiveRemoveStrangeAttachedElements(elementObject);

		// resort the list
		resortElements();
	}

	/*!
	  \internal
	  This function is used internal to update the list of all the elements. This list is implemented by a std::vector sorted on qualifiedName.
	  This function is called whenever new elements are added, removed, names are changed, parents are changed, ...
	*/
	void UMLDiagram::UMLDiagramPrivate::resortElements()
	{
		std::sort(
				_elements.begin(),
				_elements.end(),
				boost::bind(&ElementObject::qualifiedName, _1) < boost::bind(&ElementObject::qualifiedName, _2)
				);
	}

	/*!
	  \internal
	 This function is a helper function that tries to find an element with a certain qualifiedName in the list an returns an iterator
	 to this list. If the iterator is equal to _elements.end(), the element is not found.
	*/
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

	/*!
	  \internal
	  An overloaded method that tries to find the iterator to an element in _elements.
	*/
	std::vector<ElementObject*>::iterator UMLDiagram::UMLDiagramPrivate::findElement(ElementObject * elementObject)
	{
		if(elementObject == 0)
			return _elements.end();
		else
			return findElement(elementObject->qualifiedName());

	}

	/*!
	  \internal
	  This internal method attaches an element \c elementObject to this diagram and calls this function also on all it's children. This is a helper
	  method for UMLDiagram::UMLDiagramPrivate::attachElement.
	*/
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

	/*!
	  \internal
	  This internal method detaches an element \c elementObject from this diagram an calls this function also on all it's children. This is a helper
	  method for UMLDiagram::UMLDiagramPrivate::detachElement.
	*/
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

	/*!
	  \internal
	  This internal helper method removes \c elementObject from the list of elements
	*/
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

	/*!
	  \internal
	  This internal helper method add \c elementObject to the list of elements and if there exists an element with same
	  qualifiedName, destroys it.
	*/
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

	/*!
	  \internal
	  This internal helper method checks for problems with related elements for this \c elementObject.
	  It checks wether the related elements are in the same umlDiagram and otherwise zeroes out the connection.
	  This function is executed recursively when an element is detached from the UMLDiagram.
	*/
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
}
