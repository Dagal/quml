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

#include "classdiagramcontroller.hpp"
#include "_classdiagramcontroller.hpp"
#include "umldiagram.hpp"
#include "elementobject.hpp"
#include "classobject.hpp"
#include "methodobject.hpp"
#include "packageobject.hpp"

inline bool checkParentForClass(ElementObject * parent)
{
	return parent == 0 || parent->type() == Element_Class || parent->type() == Element_Package;
}

bool checkForParentRecursion(ElementObject * child, ElementObject * newParent)
{
	if(child == 0 || newParent == 0)
		return true;

	std::vector<const ElementObject*> ancestors = newParent->ancestors();

	return (std::find(
			ancestors.begin(),
			ancestors.end(),
			child) == ancestors.end());

}

ClassDiagramController::ClassDiagramController(UMLDiagram * diagram)
	: _dd(new ClassDiagramControllerPrivate(diagram))
{
}

UMLDiagram * ClassDiagramController::diagram() const
{
	return _dd->_diagram;
}

Error ClassDiagramController::detachElement(const std::string & qualifiedElementName, ElementObject ** elementObject)
{
	ElementObject * element = getElement(qualifiedElementName);
	if(elementObject != 0) *elementObject = element;

	if(!element) return Error_ElementUndefined;

	element->setUMLDiagram(0);
	element->setParent(0);

	return Error_NoError;
}

Error ClassDiagramController::renameElement(const std::string & qualifiedElementName, const std::string & newName)
{
	ElementObject * element = getElement(qualifiedElementName);
	if(!element) return Error_ElementUndefined;

	if(newName.empty()) return Error_ElementNameEmpty;

	// check for the names
	if(!checkNameAgainstSiblings(element, newName, element->parent()))
		return Error_ElementNameAlreadyUsed;

	// perform the operation
	element->setName(newName);

	return Error_NoError;
}

Error ClassDiagramController::createClass(const std::string & className, const std::string & qualifiedParentName, ClassObject ** elementObject)
{
	// useful new name?
	if(className.empty()) return Error_ElementNameEmpty;

	ElementObject * parentElement = getElement(qualifiedParentName);

	// check for the names
	if(!checkNameAgainstSiblings(0, className, parentElement))
		return Error_ElementNameEmpty;

	// check for the right type of parent
	if(!checkParentForClass(parentElement))
		return Error_ElementParentBadContainer;

	// perform the operation
	ClassObject * element = new ClassObject(className);
	element->setParent(parentElement);
	element->setUMLDiagram(diagram());

	// should we hand over the pointer?
	if(elementObject != 0)
		(*elementObject) = element;

	return Error_NoError;
}

Error ClassDiagramController::moveClass(const std::string & classQualifiedName, const std::string & newParentQualifiedName)
{
	// useful name?
	if(classQualifiedName.empty()) return Error_ElementNameEmpty;

	// check if we found a class
	ClassObject * classObject = getElement<ClassObject>(classQualifiedName);
	if(!classObject) return Error_ElementUndefined;

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the right type of parent
	if(!checkParentForClass(parentElement))
		return Error_ElementParentBadContainer;

	// check for recursion
	if(!checkForParentRecursion(classObject, parentElement))
		return Error_ElementParentRecursion;

	// perform the operation
	classObject->setParent(parentElement);

	return Error_NoError;
}

Error ClassDiagramController::createPackage(const std::string & packageName, const std::string & newParentQualifiedName, PackageObject ** elementObject)
{
	// useful name?
	if(packageName.empty()) return Error_ElementNameEmpty;

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the names
	if(!checkNameAgainstSiblings(0, packageName, parentElement))
		return Error_ElementNameEmpty;

	// check for the right type of parent
	if(parentElement != 0 && parentElement->type() != Element_Package)
		return Error_ElementParentBadContainer;

	// perform the operation
	PackageObject * packageObject = new PackageObject(packageName);
	packageObject->setParent(parentElement);
	packageObject->setUMLDiagram(diagram());

	// should we hand over the pointer?
	if(elementObject != 0)
		(*elementObject) = packageObject;

	return Error_NoError;
}

Error ClassDiagramController::movePackage(const std::string & packageQualifiedName, const std::string & newParentQualifiedName)
{
	// useful name?
	if(packageQualifiedName.empty()) return Error_ElementNameEmpty;

	// check if we found a package?
	PackageObject * packageObject = getElement<PackageObject>(packageQualifiedName);
	if(!packageObject) return Error_ElementUndefined;

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the right type of parent
	if(parentElement != 0 && parentElement->type() != Element_Package)
		return Error_ElementParentBadContainer;

	// check for recursion
	if(!checkForParentRecursion(packageObject, parentElement))
		return Error_ElementParentRecursion;

	// perform the operation
	packageObject->setParent(parentElement);

	return Error_NoError;

}

ElementObject * ClassDiagramController::getElement(const std::string & qualifiedName) const
{
	return diagram()->findElement(qualifiedName);
}

std::vector<ElementObject *> ClassDiagramController::getElements(const std::string & name, const std::string & parentQualifiedName) const
{
	return getElements(name, getElement(parentQualifiedName));
}

std::vector<ElementObject *> ClassDiagramController::getElements(const std::string & name, ElementObject * parentObject) const
{
	if(parentObject != 0 && parentObject->umlDiagram() != diagram())
		return std::vector<ElementObject*>();

	typedef std::vector<ElementObject*> elementvct;

	boost::function<bool (ElementObject *)> conditionChecker;
	const elementvct * toCheckVCT = 0;
	elementvct targetVCT;

	if(parentObject != 0)
	{
		// find in parents
		toCheckVCT = &(parentObject->children());
		targetVCT.reserve(toCheckVCT->size());
		conditionChecker = boost::bind(&ElementObject::name, _1) == boost::cref(name);
	}
	else
	{
		// find in all elements (parentless)
		toCheckVCT = &(diagram()->allElements());
		targetVCT.reserve(toCheckVCT->size());
		conditionChecker =(
				boost::bind(&ElementObject::name, _1) == boost::cref(name) &&
				boost::bind(&ElementObject::parent, _1) == (ElementObject*)0);
	}


	// copy from elements
	elementvct::iterator i =
			std::remove_copy_if(
				toCheckVCT->begin(),
				toCheckVCT->end(),
				targetVCT.begin(),
				conditionChecker
				);

	targetVCT.erase(i, targetVCT.end());
	return targetVCT;
}

bool ClassDiagramController::checkNameAgainstSiblings(ElementObject * element, const std::string & newName, ElementObject * parentObject) const
{
	if(newName.empty())
		return false;

	// check for the names
	std::vector<ElementObject *> sameNamedElements = getElements(newName, parentObject);
	if(!sameNamedElements.empty())
	{
		// not empty, than element should be a methodObject
		if(element != 0 && !element_castable<MethodObject>(element))
			return false;

		// and all the found children should also be a methodObject
		if(std::find_if(
				sameNamedElements.begin(),
				sameNamedElements.end(),
				!boost::bind(element_castable<MethodObject>, _1)
						)
			== sameNamedElements.end())
			return false;
	}

	return true;
}
