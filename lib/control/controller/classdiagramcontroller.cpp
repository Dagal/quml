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
#include "classdiagramrules.hpp"

using namespace element;
using namespace controller;

namespace
{
	typedef boost::shared_ptr<controller::IClassDiagramRules> rulePtr;

	QMap<element::ElementType, rulePtr > initRules()
	{
		QMap<element::ElementType, boost::shared_ptr<IClassDiagramRules> > ruleMap;

		ruleMap[element::Element] = rulePtr(new controller::DefaultClassDiagramRules);
		ruleMap[element::Element_Class] = rulePtr(new controller::ClassObjectDiagramRules);
		ruleMap[element::Element_Method] = rulePtr(new controller::MethodObjectDiagramRules);
		ruleMap[element::Element_Operation] = rulePtr(new controller::OperationObjectDiagramRules);
		ruleMap[element::Element_Package] = rulePtr(new controller::PackageObjectDiagramRules);
		ruleMap[element::Element_Property] = rulePtr(new controller::PropertyObjectDiagramRules);

		return ruleMap;
	}
}

namespace controller
{
	/*!
	  The default constructor for a ClassDiagramController. Note that ClassDiagramController works on the same
	  \c diagram, but does not take control over that diagram.
	*/
	ClassDiagramController::ClassDiagramController()
		: _rules(initRules())
	{
	}

	/*!
	  This overloaded method returns a rulechecker for the element supplied \c elementObject. If \c elementObject is not specified, it returns a zero
	  pointer. Otherwise the best possible rule checker is returned.
	*/
	const IClassDiagramRules * ClassDiagramController::RulesFor(element::ElementObject * elementObject) const
	{
		if(elementObject == 0)
			return 0;

		return ClassDiagramController::RulesFor(elementObject->type());
	}

	/*!
	  This overloaded method returns a rulechecker for the element type \c elementType. The best possible ruleCheckerType is returned.
	*/
	const IClassDiagramRules * ClassDiagramController::RulesFor(element::ElementType elementType) const
	{
		if(_rules.contains(elementType))
			return _rules[elementType].get();
		else
			return _rules[element::Element].get();
	}
}
