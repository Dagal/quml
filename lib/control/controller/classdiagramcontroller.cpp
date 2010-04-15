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

ElementObject * helperDetachElement(ElementObject * element)
{
	if(element == 0)
		return 0;

	element->setUMLDiagram(0);
	element->setParent(0);

	return element;
}

ClassDiagramController::ClassDiagramController(UMLDiagram * diagram)
	: _dd(new ClassDiagramControllerPrivate(diagram))
{
}

UMLDiagram * ClassDiagramController::diagram() const
{
	return _dd->_diagram;
}

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

Error ClassDiagramController::createClass(const QString & className, const QString & qualifiedParentName, ClassObject ** elementObject)
{
	CreateAction a;

	// useful new name?
	if(className.isEmpty()) SEND_AND_RETURN(Error_ElementNameEmpty);

	ElementObject * parentElement = getElement(qualifiedParentName);

	// check for the names
	if(!checkNameAgainstSiblings(0, className, parentElement))
		SEND_AND_RETURN(Error_ElementNameEmpty);

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
  \todo: Update the ClassDiagramController::checkNameAgainstSiblings to use correct methodObject checking
  */
bool ClassDiagramController::checkNameAgainstSiblings(ElementObject * element, const QString & newName, ElementObject * parentObject) const
{
	if(newName.isEmpty())
		return false;

	// check for the names
	QList<ElementObject *> sameNamedElements = getElements(newName, parentObject);
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
