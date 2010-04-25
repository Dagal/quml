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
		_rules[element::Element] = rulePtr();
		_rules[element::Element_Class] = rulePtr(new ClassObjectDiagramRules(_controller));
		_rules[element::Element_Method] = rulePtr(new MethodObjectDiagramRules(_controller));
		_rules[element::Element_Operation] = rulePtr(new OperationObjectDiagramRules(_controller));
		_rules[element::Element_Package] = rulePtr(new PackageObjectDiagramRules(_controller));
		_rules[element::Element_Property] = rulePtr(new PropertyObjectDiagramRules(_controller));
	}

	template <class T> IClassDiagramRules * getRulesFor()
	{
		if(_rules.contains(T::elementtype))
			return _rules[T::elementtype].get();
		else
			return 0;
	}
	IClassDiagramRules * getRulesFor(element::ElementObject * elementObject)
	{
		if(elementObject == 0)
			return 0;

		if(_rules.contains(elementObject->type()))
			return _rules[elementObject->type()].get();
		else
			return 0;
	}


	ClassDiagramController * const _controller;
	element::UMLDiagram * const _diagram;
	QMap<element::ElementType, boost::shared_ptr<IClassDiagramRules> > _rules;
};

#endif // _CLASSDIAGRAMCONTROLLER_HPP
