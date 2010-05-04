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
#include "operationobject.hpp"
#include "parameter.hpp"
#include <QVector>

using namespace element;

#define SEND_AND_RETURN(ERROR) \
{\
	a.error = ERROR;\
	_actionListener.sendMessage(a); \
	return ERROR; \
}


bool isNewParentRecursive(ElementObject * child, ElementObject * newParent)
{
	if(child == 0 || newParent == 0 || child == newParent)
		return false;

	QList<ElementObject*> ancestors = newParent->ancestors();

	return (std::find(
			ancestors.begin(),
			ancestors.end(),
			child) != ancestors.end());
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
	: _dd(new ClassDiagramControllerPrivate(this, diagram ? diagram : new UMLDiagram()))
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
  This function tries to rename an element with a certain \c qualifiedElementName to \c newName.

  Possible return types are:
  \li Error_ElementUndefined: No element with this qualifiedName could be found
  \li Error_ElementNameInvalid: The specified name is invalid for this element
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

	IClassDiagramRules * rule = _dd->getRulesFor(a.elementObject);

	if(!rule->isValidName(newName))
		SEND_AND_RETURN(Error_ElementNameInvalid);

	// check for the names
	if(!rule->isNameValidWithinParent(a.elementObject, a.elementObject->parent(), newName))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	a.elementObject->setName(newName);

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This function tries to move an element with a certain \c qualifiedElementName to a new parent with name \c newParentQualfiedName

  Possible return types are:
  \li Error_ElementUndefined: There is no ElementObject with this qualifiedElementName
  \li Error_ElementParentBadContainer: The newParentQualifiedName is not the right type to store this element in
  \li Error_ElementParentRecursion: By performing this operation the umlDiagram would contain recursion.
  \li Error_ElementNameAlreadyUsed: The parent element already contains an element with that name.
  \li Error_NoError: The element was succesfully moved
*/
Error ClassDiagramController::moveElement(const QString & qualifiedElementName, const QString & newParentQualifiedName)
{
	MoveAction a;

	a.elementObject = getElement(qualifiedElementName);
	if(!a.elementObject)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.oldParent = a.elementObject->parent();
	ElementObject * newParent = getElement(newParentQualifiedName);

	IClassDiagramRules * rule = _dd->getRulesFor(a.elementObject);

	// check for the right container type
	if(!rule->isParentRightContainerType(newParent))
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check for no recursion
	if(isNewParentRecursive(a.elementObject, newParent))
		SEND_AND_RETURN(Error_ElementParentRecursion);

	// check for the right name
	if(!rule->isNameValidWithinParent(a.elementObject, newParent, a.elementObject->name()))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	a.elementObject->setParent(newParent);

	SEND_AND_RETURN(Error_NoError);
}


/*!
  This function tries to move and rename a certain \c qualifiedElementName to a new parent with name \c
  newParentQualifiedName and renames the element to \c newName.

  Possible return types are:
  \li Error_ElementUndefined: There is no ElementObject with this qualifiedName
  \li Error_ElementNameInvalid: The specified new name is invalid for this element
  \li Error_ElementParentBadContainer: The newParentQualifedName is tno the right type to store this element in
  \li Error_ElementParentRecursion: By performing this operation the umlDiagram wuold contain recursion
  \li Error_ElementNameAlreadyUsed: The new parent already contains an element  with new name
  \li Error_NoError: The element was succesfully removed
*/
Error ClassDiagramController::moveAndRenameElement(const QString & qualifiedElementName, const QString & newParentQualifiedName, const QString & newName)
{
	MoveAndRenameAction a;

	a.elementObject = getElement(qualifiedElementName);

	if(!a.elementObject)
		SEND_AND_RETURN(Error_ElementUndefined);

	a.oldParent = a.elementObject->parent();
	a.oldName = a.elementObject->name();
	ElementObject * newParent = getElement(newParentQualifiedName);

	IClassDiagramRules * rule = _dd->getRulesFor(a.elementObject);

	// check for right name
	if(!rule->isValidName(newName))
		SEND_AND_RETURN(Error_ElementNameInvalid);

	// check for the right container type
	if(!rule->isParentRightContainerType(newParent))
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check for no recursion
	if(isNewParentRecursive(a.elementObject, newParent))
		SEND_AND_RETURN(Error_ElementParentRecursion);

	// check for the right name
	if(!rule->isNameValidWithinParent(a.elementObject, newParent, a.elementObject->name()))
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	a.elementObject->setUMLDiagram(0);
	a.elementObject->setName(newName);
	a.elementObject->setParent(newParent);

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This function tries to create a ClassObject with \c className in the container \c qualifiedParentName.
  If the qualifiedParentName is empty or not found, the class is created with no parent. If the double pointer
  \c elementObject is not zero, the new ClassObject is stored herein.

  possible returns types are:
  \li Error_ElementNameInvalid: The className is invalid
  \li Error_ElementParentBadContainer: The qualifiedParentName is not the right type to store a class in
  \li Error_ElementNameAlreadyUsed: Creating an element with this name would give conflict with another element
  \li Error_NoError: The class was sucessfully created
*/
Error ClassDiagramController::createClass(const QString & className, const QString & qualifiedParentName, ClassObject ** elementObject)
{
	return _dd->CreateElement<ClassObject>(className, qualifiedParentName, elementObject);
}


/*!
  This function tries to create a PackageObject with \c packageName in the container \c qualifiedParentName.
  If the qualifiedParentName is empty or not found, the package is created with no parent. If the double pointer
  \c elementObject is not zero, the new PackageObject is stored herein.

  possible returns types are:
  \li Error_ElementNameInvalid: The packageName is invalid.
  \li Error_ElementNameAlreadyUsed: Creating an element with this name would give conflict with another element
  \li Error_ElementParentBadContainer: The qualifiedParentName is not the right type to store a package in
  \li Error_NoError: The package was sucessfully created
*/
Error ClassDiagramController::createPackage(const QString & packageName, const QString & qualifiedParentName, PackageObject ** elementObject)
{
	return _dd->CreateElement<PackageObject>(packageName, qualifiedParentName, elementObject);
}

/*!
  This function tries to create a OperationObject with \c operationName in the container \c qualifiedParentName.
  If the double pointer \c operationObject is not zero, the OperationObject is stored herein.

  possible return types are:
  \li Error_ElementNameInvalid: The operationName is invalid.
  \li Error_ElementNameAlreadyUsed: Creating an element with this name would give conflict with another element.
  \li Error_ElementParentBadContainer: The qualifiedParentName is not the right type to store a operation in.
  \li Error_ParameterListUndefinedTypes: The supplied parameterlist does contain undefined types.
  \li Error_ParameterListWrongOrder: The parameterlist contains element with no default value after elements with a default value.
  \li Error_NoError: The operation was succesfully created
*/
Error ClassDiagramController::createOperation(const QString & operationName, const QString & qualifiedParentName, const ParameterList & parameters, OperationObject ** operationObject)
{
	CreateAction a;

	IClassDiagramRules * rule = _dd->getRulesFor(Element_Operation);

	// useful new name?
	if(!rule->isValidName(operationName))
		SEND_AND_RETURN(Error_ElementNameInvalid);

	element::ElementObject * parentElement = getElement(qualifiedParentName);

	// check for the right type of parent
	if(!rule->isParentRightContainerType(parentElement))
		SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check the parameter list
	Error parameterListError = checkParameterList(parameters);
	if(parameterListError != Error_NoError)
		SEND_AND_RETURN(parameterListError);

	// we should already create the operation to  store the paramaters
	OperationObject * operation = new OperationObject(operationName);
	operation->setParameterList(parameters);

	// check for the names
	if(!rule->isNameValidWithinParent(operation, parentElement, operationName))
	{
		delete operation;
		SEND_AND_RETURN(Error_ElementNameAlreadyUsed);
	}

	// perform the operation
	operation->setParent(parentElement);
	operation->setUMLDiagram(diagram());

	a.elementObject = operation;

	// should we hand over the pointer?
	if(operationObject != 0)
		(*operationObject) = operation;

	SEND_AND_RETURN(Error_NoError);
}

/*!
  This method returns an ElementObject with a certain qualifiedName.
*/
ElementObject * ClassDiagramController::getElement(const QString & qualifiedName) const
{
	return diagram()->findElement(qualifiedName);
}

/*!
  Finds all the elements in the element with name \c parentQualifiedName with a certain name. If the \c
  parentQualifiedName is empty or not used, all elements with no parent and name \c name are returned.
*/
QList<ElementObject *> ClassDiagramController::getElements(const QString & name, const QString & parentQualifiedName) const
{
	return getElements(name, getElement(parentQualifiedName));
}

/*!
  Finds all the elements in the element \c parentObject with a certain name. If the \c parentObject is
  zero, all elements with no parent and name \c name are returned.
*/
QList<ElementObject *> ClassDiagramController::getElements(const QString & name, ElementObject * parentObject) const
{
	typedef QVector<ElementObject*> elementvct;
	typedef QList<ElementObject*> elementlst;

	if(parentObject != 0 && parentObject->umlDiagram() != diagram())
		return elementlst();

	const elementlst * toCheckVCT = 0;

	elementvct targetVCT;

	if(parentObject != 0)
		// find in parents
		toCheckVCT = &(parentObject->children());
	else
		// find in all elements (parentless)
		toCheckVCT = &(diagram()->upperLevelElements());


	// reserve enough space in the target vector
	targetVCT.reserve(toCheckVCT->size());


	// copy from elements
	elementvct::iterator i =
			std::remove_copy_if(
					toCheckVCT->begin(),
					toCheckVCT->end(),
					targetVCT.begin(),
					boost::bind(&ElementObject::name, _1) == boost::cref(name)
					);

	targetVCT.erase(i, targetVCT.end());

	return elementlst::fromVector(targetVCT);
}


/*!
  This methods checks whether the given list is correct.
  Possible returns types are:
  \li Error_NoError: the list is okay
  \li Error_ParameterListWrongOrder: There are parameters without default values after parameters with default values
  \li Error_ParameterListUndefinedTypes: Some of the parameters don't have a valid dataType
*/
Error ClassDiagramController::checkParameterList(const ParameterList & list) const
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
bool ClassDiagramController::AreParameterListsSimilar(const ParameterList & listA, const ParameterList & listB) const
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


IClassDiagramRules * ClassDiagramController::ClassDiagramControllerPrivate::getRulesFor(element::ElementType elementType) const
{
	if(_rules.contains(elementType))
		return _rules[elementType].get();
	else
		return _rules[element::Element].get();
}

IClassDiagramRules * ClassDiagramController::ClassDiagramControllerPrivate::getRulesFor(element::ElementObject * elementObject) const
{
	if(elementObject == 0)
		return 0;

	if(_rules.contains(elementObject->type()))
		return _rules[elementObject->type()].get();
	else
		return _rules[element::Element].get();
}
