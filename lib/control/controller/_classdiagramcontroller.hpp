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

#ifndef _CLASSDIAGRAMCONTROLLER_HPP
#define _CLASSDIAGRAMCONTROLLER_HPP

#define INT_SEND_AND_RETURN(ERROR) \
{\
	a.error = ERROR;\
	_controller->actionListener().sendMessage(a); \
	return ERROR; \
}

#include "classdiagramcontroller.hpp"
#include "iclassdiagramrules.hpp"
#include "classdiagramrules.hpp"

struct ClassDiagramController::ClassDiagramControllerPrivate
{
	typedef boost::shared_ptr<IClassDiagramRules> rulePtr;

	ClassDiagramControllerPrivate(ClassDiagramController * controller, element::UMLDiagram * diagram)
		: _controller(controller)
		, _diagram(diagram)
	{
		// fill all the rules for the classDiagram
		_rules[element::Element] = rulePtr(new DefaultClassDiagramRules(_controller));
		_rules[element::Element_Class] = rulePtr(new ClassObjectDiagramRules(_controller));
		_rules[element::Element_Method] = rulePtr(new MethodObjectDiagramRules(_controller));
		_rules[element::Element_Operation] = rulePtr(new OperationObjectDiagramRules(_controller));
		_rules[element::Element_Package] = rulePtr(new PackageObjectDiagramRules(_controller));
		_rules[element::Element_Property] = rulePtr(new PropertyObjectDiagramRules(_controller));
	}

	IClassDiagramRules * getRulesFor(element::ElementType elementType) const;
	IClassDiagramRules * getRulesFor(element::ElementObject * elementObject) const;

	template <class T> Error CreateElement(const QString & name, const QString & newParentQualifiedName, T ** elementObject) const;

	ClassDiagramController * const _controller;
	element::UMLDiagram * const _diagram;
	QMap<element::ElementType, boost::shared_ptr<IClassDiagramRules> > _rules;
};

template <class T> Error ClassDiagramController::ClassDiagramControllerPrivate::CreateElement(const QString & name, const QString & qualifiedParentName, T ** elementObject) const
{
	CreateAction a;

	IClassDiagramRules * rule = getRulesFor(static_cast<element::ElementType>(T::elementtype));

	// useful new name?
	if(!rule->isValidName(name))
		INT_SEND_AND_RETURN(Error_ElementNameInvalid);

	element::ElementObject * parentElement = _controller->getElement(qualifiedParentName);

	// check for the right type of parent
	if(!rule->isParentRightContainerType(parentElement))
		INT_SEND_AND_RETURN(Error_ElementParentBadContainer);

	// check for the names
	if(!rule->isNameValidWithinParent(0, parentElement, name))
		INT_SEND_AND_RETURN(Error_ElementNameAlreadyUsed);

	// perform the operation
	T * element = new T(name);
	element->setParent(parentElement);
	element->setUMLDiagram(_diagram);

	a.elementObject = element;

	// should we hand over the pointer?
	if(elementObject != 0)
		(*elementObject) = element;

	INT_SEND_AND_RETURN(Error_NoError);
}

#endif // _CLASSDIAGRAMCONTROLLER_HPP
