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

	template <typename T> T highestBit(T n)
	{
		n |= (n >>  1);
		n |= (n >>  2);
		n |= (n >>  4);
		n |= (n >>  8);
		n |= (n >> 16);
		return n - (n >> 1);
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
		int val = elementType;

		while(val != 0)
		{
			// try for the current value
			element::ElementType cur =  static_cast<element::ElementType>(val);
			if(_rules.contains(cur))
				return _rules[cur].get();

			// try for the highest bit
			cur = static_cast<element::ElementType>(highestBit(val));
			if(cur != val && _rules.contains(cur))
				return _rules[cur].get();

			// no, remove highest bit
			val ^= cur;
		}

		return _rules[element::Element].get();
	}

	/*!
	  Use this method to add a new rulechecker for the element with type \c elementType. The controller takes control over the \c rule.
	*/
	void ClassDiagramController::addRule(element::ElementType elementType, IClassDiagramRules * rule)
	{
		if(rule == 0)
			_rules.remove(elementType);
		else
			_rules[elementType] = rulePtr(rule);
	}
}
