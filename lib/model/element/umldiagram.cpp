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

//	// is it a method?
//	MethodObject * method = element_cast<MethodObject>(element);
//	if(method) return method->returnType();
//
//	// is it a parameter?
//	ParameterObject * param = element_cast<ParameterObject>(element);
//	if(param) return param->datatype();

	return std::string();
}

UMLDiagram::UMLDiagram()
		: _dd(new UMLDiagram::UMLDiagramPrivate)
{
}

Error UMLDiagram::attachElement(boost::shared_ptr<ElementObject> elementObject, const std::string & parentQualifiedName)
{
	// existing element?
	if(elementObject.get() == 0)
		return Error_ElementUndefined;

	UMLDiagram * oldDiagram = elementObject->umlDiagram();

	// attached to this umlDiagram?
	if(oldDiagram == this)
		return Error_NoError;

	// attached to another umlDiagram?
	if(oldDiagram != 0)
		return Error_UMLDiagramWrong;

	// attach it to this diagram
	elementObject->_dd->_diagram = this;

	// set the new parent, using an empty element (accidental-related-elements-hijacking)
	std::string elementName = elementObject->name();
	elementObject->_dd->_parent = 0;
	elementObject->_dd->_name = "";
	Error error;
	if((error = changeParent(elementObject.get(), parentQualifiedName, elementName)) != Error_NoError)
		return error;

	// add the new element to the list
	_dd->addNewElement(elementObject);

	return Error_NoError;
}

boost::shared_ptr<ElementObject> UMLDiagram::detachElement(const std::string & qualifiedName)
{
	boost::shared_ptr<ElementObject> elementPtr = _dd->removeElement(qualifiedName);

	// check for not found
	if(elementPtr.get() == 0)
		return elementPtr;

	// send the message to the old parent
	if(elementPtr->parent()) elementPtr->parent()->onChildRemoved(elementPtr.get());

	// set the umlDiagram and parent to 0
	elementPtr->_dd->_parent = 0;
	elementPtr->_dd->_diagram = 0;

	return elementPtr;
}

Error UMLDiagram::changeParent(ElementObject * element, const std::string & newParentQualifiedName, const std::string & newElementName)
{
	if(element == 0)
		return Error_ElementUndefined;

	ElementObject * newParent = findElement(newParentQualifiedName);

	// is it useful to do this?
	if(element->parent() == newParent)
		return Error_NoError;

	std::string realElementName = newElementName;

	// name changed?
	if(realElementName.empty())
		realElementName = element->name();

	// can we add the element here?
	if(!findChildren(newParent, realElementName).empty())
		return Error_ElementNameAlreadyUsed;

	// store the oldName, to update the relatedElements[KEY];
	std::string oldQualifiedName = element->qualifiedName();
	ElementObject * oldParent = element->parent();

	// set the new parent & change the name
	element->_dd->_name = realElementName;
	element->_dd->_parent = newParent;

	// send the messages
	if(oldParent) oldParent->onChildRemoved(element);
	if(newParent) newParent->onChildAdded(element);

	// update the related elements, KEY side
	_dd->updateRelatedKeyElement(oldQualifiedName, element->qualifiedName());

	return Error_NoError;
}

Error UMLDiagram::changeName(ElementObject * element, const std::string & newElementName)
{
	// element is useful?
	if(!element)
		return Error_ElementUndefined;

	// is the name okay?
	if(newElementName.empty())
		return Error_ElementNameEmpty;

	// can we rename the element?
	if(!findChildren(element->parent(), newElementName).empty())
		return Error_ElementNameAlreadyUsed;

	// change the name
	std::string oldQualifiedName = element->qualifiedName();
	element->_dd->_name = newElementName;

	// update the related elements, KEY side
	_dd->updateRelatedKeyElement(oldQualifiedName, element->qualifiedName());

	return Error_NoError;
}

ElementObject * UMLDiagram::findElement(const std::string & qualifiedName) const
{
	UMLDiagramPrivate::element_vector::iterator i = _dd->findElement(qualifiedName);
	if(i == _dd->_elements.end())
		return 0;
	else
		return (*i).get();
}

std::vector<ElementObject*> UMLDiagram::allElements() const
{
	std::vector<ElementObject*> elements(_dd->_elements.size());

	std::transform(
			_dd->_elements.begin(),
			_dd->_elements.end(),
			elements.begin(),
			boost::bind(&boost::shared_ptr<ElementObject>::get, _1));

	return elements;
}

std::vector<ElementObject *> UMLDiagram::findRelatedElements(const std::string & qualifiedName) const
{
	UMLDiagramPrivate::relatedElements_map::iterator i = _dd->_relatedElements.find(qualifiedName);
	if(i == _dd->_relatedElements.end())
		return std::vector<ElementObject*>();
	else
		return (*i->second);

	boost::shared_ptr<std::vector<ElementObject*> > a;
}

void UMLDiagram::UMLDiagramPrivate::addNewElement(boost::shared_ptr<ElementObject> elementObject)
{
	// push it in the list
	_elements.push_back(elementObject);

	// resort everything
	resortElements();

	// add it to the relatedElements if necessary
	addRelatedValueElement(elementToRelatedQualifiedName(elementObject.get()), elementObject.get());
}

boost::shared_ptr<ElementObject> UMLDiagram::UMLDiagramPrivate::removeElement(const std::string & qualifiedName)
{
	// find the element
	element_vector::iterator i = findElement(qualifiedName);
	if(i == _elements.end())
		return boost::shared_ptr<ElementObject>();

	// take it from the list
	boost::shared_ptr<ElementObject> elementPtr = *i;
	_elements.erase(i);

	// remove from the relatedElements, Value side
	removeRelatedValueElement(elementToRelatedQualifiedName(elementPtr.get()), elementPtr.get());

	return elementPtr;
}

void UMLDiagram::UMLDiagramPrivate::updateRelatedValueElement(ElementObject * elementObject, const std::string & oldQualifiedName, const std::string & newQualifiedName)
{
	if(elementObject == 0)
		return;

	removeRelatedValueElement(oldQualifiedName, elementObject);
	addRelatedValueElement(newQualifiedName, elementObject);
}

void UMLDiagram::UMLDiagramPrivate::updateRelatedKeyElement(const std::string & oldQualifiedName, const std::string & newQualifiedName)
{
	// is it useful?
	if(oldQualifiedName.empty() || oldQualifiedName == newQualifiedName)
		return;

	// find the old value
	relatedElements_map::iterator i = _relatedElements.find(oldQualifiedName);
	if(i == _relatedElements.end())
		return;

	// remove from old  position
	boost::shared_ptr<std::vector<ElementObject*> > ptr = i->second;
	_relatedElements.erase(i);

	// useful to continue?
	if(i->second == 0 || i->second->empty() || newQualifiedName.empty())
		return;

	// insert into list
	_relatedElements[newQualifiedName] = ptr;
}

UMLDiagram::UMLDiagramPrivate::element_vector::iterator UMLDiagram::UMLDiagramPrivate::findElement(const std::string & qualifiedName)
{
	using namespace stf;

	return binary_find_if(
			_elements.begin(),
			_elements.end(),
			boost::bind<int>(elementFinder, _1, boost::cref(qualifiedName))
			);
}

void UMLDiagram::UMLDiagramPrivate::resortElements()
{
	std::sort(
			_elements.begin(),
			_elements.end(),
			elementComparator
			);
}

void UMLDiagram::UMLDiagramPrivate::removeRelatedValueElement(const std::string & oldQualifiedName, ElementObject * element)
{
	if(oldQualifiedName.empty() || element == 0)
		return;

	// find the element
	relatedElements_map::iterator i = _relatedElements.find(oldQualifiedName);
	if(i == _relatedElements.end())
		return;

	// is it useful to do this?
	if(i->second == 0)
	{
		_relatedElements.erase(i);
		return;
	}

	// get the pointer to the vector
	std::vector<ElementObject*> & vct = *(i->second);

	// remove from the list if possible
	std::vector<ElementObject*>::iterator j = std::find(vct.begin(), vct.end(), element);
	if(j != vct.end())
		vct.erase(j);

	// should it be clear?
	if(vct.empty()) _relatedElements.erase(i);

	return;
}

void UMLDiagram::UMLDiagramPrivate::addRelatedValueElement(const std::string & newQualifiedName, ElementObject * element)
{
	// useful information?
	if(newQualifiedName.empty() || element == 0)
		return;

	boost::shared_ptr<std::vector<ElementObject*> > vctPtr;

	// can we find a vector?
	relatedElements_map::iterator i = _relatedElements.find(newQualifiedName);
	if(i == _relatedElements.end())
	{
		vctPtr = boost::shared_ptr<std::vector<ElementObject*> >(new std::vector<ElementObject*>(1));
		_relatedElements[newQualifiedName] = vctPtr;
	}
	else
		vctPtr = i->second;

	vctPtr->push_back(element);
}

