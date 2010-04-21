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
#include "parameter.hpp"
#include <QVector>

using namespace element;

#define SEND_AND_RETURN(ERROR) \
{\
	a.error = ERROR;\
	_actionListener.sendMessage(a); \
	return ERROR; \
}

inline bool checkParentForClass(ElementObject * parent)
{
	return parent == 0 || parent->type() == Element_Class || parent->type() == Element_Package;
}

bool checkForParentRecursion(ElementObject * child, ElementObject * newParent)
{
	if(child == 0 || newParent == 0 || child == newParent)
		return true;

	QList<ElementObject*> ancestors = newParent->ancestors();

	return (std::find(
			ancestors.begin(),
			ancestors.end(),
			child) == ancestors.end());

}

template <class T> canAttachRenamedElement(T * elementObject, const QString & newName, QList<ElementObject *> & children)
{

}

ElementObject * helperDetachElement(ElementObject * element)
{
	if(element == 0)
		return 0;

	element->setUMLDiagram(0);
	element->setParent(0);

	return element;
}

/*!
  The default constructor for a ClassDiagramController. Note that ClassDiagramController works on the same
  \c diagram, but does not take control over that diagram.
*/
ClassDiagramController::ClassDiagramController(UMLDiagram * diagram)
	: _dd(new ClassDiagramControllerPrivate(diagram ? diagram : new UMLDiagram()))
{
}

/*!
  Returns a pointer to the UMLDiagram
*/
UMLDiagram * ClassDiagramController::diagram() const
{
	return _dd->_diagram;
}

/*!
  This function tries to detach an element with a certain \c qualifiedElementName from this UMLDiagram. If a double pointer
  is passed on by \c elementObject, the value of the detached element is stored herein.

  possible return types are:
  \li Error_ElementUndefined: No element with this qualifiedName could be found
  \li Error_NoError: The element was succesfully detached from this diagram
*/
Error ClassDiagramController::detachElement(const QString & qualifiedElementName, ElementObject ** elementObject)
{
	DetachAction a;

	a.elementObject = helperDetachElement(getElement(qualifiedElementName));

	if(a.elementObject == 0)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.oldParent = a.elementObject->parent();

	*elementObject = a.elementObject;

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This function tries to rename an element with a certain \c qualifiedName to \c newName.

  Possible return types are:
  \li Error_ElementUndefined: No element with this qualifiedName could be found
  \li Error_ElementNameAlreadyUsed: Renaming this element would give conflict with another element
  \li Error_NoError: The element was succesfully renamed
*/
Error ClassDiagramController::renameElement(const QString & qualifiedElementName, const QString & newName)
{
	RenameAction a;

	a.elementObject = getElement(qualifiedElementName);
	if(!a.elementObject)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.oldName = a.elementObject->name();

	if(newName.isEmpty()) SEND_AND_RETURN(Error_ElementNameEmpty);

	// check for the names
	if(!checkNameAgainstSiblings(a.elementObject, newName, a.elementObject->parent()))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	a.elementObject->setName(newName);

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This function tries to create a ClassObject with \c className in the container \c qualifiedParentName.
  If the qualifiedParentName is empty or not found, the class is created with no parent. If the double pointer
  \c elementObject is not zero, the new ClassObject is stored herein.

  possible returns types are:
  \li Error_ElementNameEmpty: The className is empty
  \li Error_ElementNameAlreadyUsed: Creating an element with this name would give conflict with another element
  \li Error_ElementParentBadContainer: The qualifiedParentName is not the right type to store a class in
  \li Error_NoError: The class was sucessfully created
*/
Error ClassDiagramController::createClass(const QString & className, const QString & qualifiedParentName, ClassObject ** elementObject)
{
	CreateAction a;

	// useful new name?
	if(className.isEmpty()) SEND_AND_RETURN(Error_ElementNameEmpty);

	ElementObject * parentElement = getElement(qualifiedParentName);

	// check for the names
	if(!checkNameAgainstSiblings(0, className, parentElement))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// check for the right type of parent
	if(!checkParentForClass(parentElement))
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// perform the operation
	ClassObject * element = new ClassObject(className);
	element->setParent(parentElement);
	element->setUMLDiagram(diagram());

	a.elementObject = element;

	// should we hand over the pointer?
	if(elementObject != 0)
		(*elementObject) = element;

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This function tries to move a class with a certain \c classQualifiedName to a new container \c newParentQualifiedName.
  If the \c newParentQualifiedName is empty or not found, the parent is set to zero.

  possible return types are:
  \li Error_ElementNameEmpty: The classQualifiedName is empty
  \li Error_ElementUndefined: There is no ClassObject with this classQualifiedName
  \li Error_ElementParentBadContainer: The newParentQualifiedName is not the right type to store a class in
  \li Error_ElementParentRecursion: By performing this operation the umlDiagram would contain recursion.
  \li Error_ElementNameAlreadyUsed: The parent element already contains an element with that name.
  \li Error_NoError: the class was succesfully moved
*/
Error ClassDiagramController::moveClass(const QString & classQualifiedName, const QString & newParentQualifiedName)
{
	MoveAction a;

	// useful name?
	if(classQualifiedName.isEmpty())
		SEND_AND_RETURN(Error_ElementNameEmpty);

	// check if we found a class
	ClassObject * classObject = getElement<ClassObject>(classQualifiedName);
	if(!classObject)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.elementObject = classObject;
	a.oldParent = classObject->parent();

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the right type of parent
	if(!checkParentForClass(parentElement))
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check for recursion
	if(!checkForParentRecursion(classObject, parentElement))
		SEND_AND_RETURN(Error_ElementParentRecursion);

	// check if we can add this class to the parent
	if(!checkNameAgainstSiblings(classObject, classObject->name(), parentElement))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	classObject->setParent(parentElement);

	SEND_AND_RETURN(Error_NoError);
}

Error ClassDiagramController::createPackage(const QString & packageName, const QString & newParentQualifiedName, PackageObject ** elementObject)
{
	CreateAction a;

	// useful name?
	if(packageName.isEmpty()) SEND_AND_RETURN(Error_ElementNameEmpty);

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the names
	if(!checkNameAgainstSiblings(0, packageName, parentElement))
		SEND_AND_RETURN(Error_ElementNameEmpty);

	// check for the right type of parent
	if(parentElement != 0 && parentElement->type() != Element_Package)
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// perform the operation
	PackageObject * packageObject = new PackageObject(packageName);
	packageObject->setParent(parentElement);
	packageObject->setUMLDiagram(diagram());
	a.elementObject = packageObject;

	// should we hand over the pointer?
	if(elementObject != 0)
		(*elementObject) = packageObject;

	SEND_AND_RETURN(Error_NoError);
}

Error ClassDiagramController::movePackage(const QString & packageQualifiedName, const QString & newParentQualifiedName)
{
	MoveAction a;

	// useful name?
	if(packageQualifiedName.isEmpty())
		SEND_AND_RETURN(Error_ElementNameEmpty);

	// check if we found a package?
	PackageObject * packageObject = getElement<PackageObject>(packageQualifiedName);
	if(!packageObject)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.elementObject = packageObject;
	a.oldParent = packageObject->parent();

	ElementObject * parentElement = getElement(newParentQualifiedName);

	// check for the right type of parent
	if(parentElement != 0 && parentElement->type() != Element_Package)
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check for recursion
	if(!checkForParentRecursion(packageObject, parentElement))
		SEND_AND_RETURN(Error_ElementParentRecursion);

	// perform the operation
	packageObject->setParent(parentElement);

	SEND_AND_RETURN(Error_NoError);

}

ElementObject * ClassDiagramController::getElement(const QString & qualifiedName) const
{
	return diagram()->findElement(qualifiedName);
}

QList<ElementObject *> ClassDiagramController::getElements(const QString & name, const QString & parentQualifiedName) const
{
	return getElements(name, getElement(parentQualifiedName));
}

QList<ElementObject *> ClassDiagramController::getElements(const QString & name, ElementObject * parentObject) const
{
	typedef QVector<ElementObject*> elementvct;
	typedef QList<ElementObject*> elementlst;

	if(parentObject != 0 && parentObject->umlDiagram() != diagram())
		return elementlst();



	boost::function<bool (ElementObject *)> conditionChecker;
	const elementlst * toCheckVCT = 0;

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

	return elementlst::fromVector(targetVCT);
}

/*!
  This method checks whether an c\ elementObject can be renamed to \c newName in the \c parentObject. If the parent is not specified, the element is
  supposed to have no parent. The elementObject cannot be zero (necessary to check its type). If you want to use this function for a object
  you want to create, check the overloaded function.
*/
bool ClassDiagramController::checkNameAgainstSiblings(ElementObject * elementObject, const QString & newName, ElementObject * parentObject) const
{
	if(elementObject == 0 || newName.isEmpty())
		return false;

	// are there elements with the same name?
	QList<ElementObject*> sameNamedElements = getElements(newName, parentObject);
	if(sameNamedElements.isEmpty())
		return true;

	// everything should be castable to a method

	// start with the basic element
	MethodObject * methodObject = element_cast<MethodObject>(elementObject);
	if(!methodObject)
		return false;

	// loop over all the elements, check if there a method and compare the parameterlists
	foreach(ElementObject * sameNamedElement, sameNamedElements)
	{
		MethodObject * tmpObject = element_cast<MethodObject>(sameNamedElement);
		if(!tmpObject)
			return false;

		if(checkForSimilarParameterLists(methodObject->parameters(), tmpObject->parameters()))
			return false;
	}

	return true;

}

/*!
  This method checks whether a ElementObject of type \c type can be created in a certain \c parentObject with a name \c newName. If the parent is
  not specified, the element is supposed to have no parent
*/
bool ClassDiagramController::checkNameAgainstSiblings(ElementType type, const QString & newName, ElementObject * parentObject) const
{
	if(newName.isEmpty())
		return false;

	// are there elements with the same name?
	QList<ElementObject*> sameNamedElements = getElements(newName, parentObject);
	if(sameNamedElements.isEmpty())
		return true;

	// everything should be castable to a method

	// start with the basic element
	if((type & Element_Method) != Element_Method)
		return false;

	element::parameterList lst;

	// loop over all the elements, check if there a method and compare the parameterlists
	foreach(ElementObject * sameNamedElement, sameNamedElements)
	{
		MethodObject * tmpObject = element_cast<MethodObject>(sameNamedElement);
		if(!tmpObject)
			return false;

		if(checkForSimilarParameterLists(lst, tmpObject->parameters()))
			return false;
	}

	return true;
}

/*!
  This methods checks whether the given list is correct.
  Possible returns types are:
  \li Error_NoError: the list is okay
  \li Error_ParameterListWrongOrder: There are parameters without default values after parameters with default values
  \li Error_ParameterListUndefinedTypes: Some of the parameters don't have a valid dataType
*/
Error ClassDiagramController::checkParameterList(const parameterList & list) const
{
	bool defaultStarted = false;

	for(int i = 0; i < list.size(); i++)
	{
		const Parameter & curParam = list[i];

		// check for order in parameters
		if(curParam.defaultValue().isEmpty() && defaultStarted)
			return Error_ParameterListWrongOrder;

		if(!curParam.defaultValue().isEmpty())
			defaultStarted = true;

		if(curParam.datatype() == 0)
			return Error_ParameterListUndefinedTypes;
	}

	return Error_NoError;
}

/*!
  This method checks whether two lists of parameters are similar. This is the case as soon as the parameters in listA and listB have the same datatype
  from the first parameter to the the last non-default parameter. The function returns true if they are similar.
*/
bool ClassDiagramController::checkForSimilarParameterLists(const element::parameterList & listA, const element::parameterList & listB) const
{
	int i = 0;

	while(i < listA.size() && i < listB.size())
	{
		if(listA[i].datatype() != listB[i].datatype())
			return false;

		if(!listA[i].defaultValue().isEmpty() || !listB[i].defaultValue().isEmpty())
			return true;

		i++;
	}

	return true;
}
