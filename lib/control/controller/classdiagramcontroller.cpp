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

namespace controller
{
	/*!
	  The default constructor for a ClassDiagramController. Note that ClassDiagramController works on the same
	  \c diagram, but does not take control over that diagram.
	*/
	ClassDiagramController::ClassDiagramController(UMLDiagram * diagram)
		: _dd(new ClassDiagramControllerPrivate(this, diagram))
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
	  This overloaded static method returns a rulechecker for the element supplied \c elementObject. If \c elementObject is not specified, it returns a zero
	  pointer. Otherwise the best possible rule checker is returned.
	*/
	const IClassDiagramRules * ClassDiagramController::RulesFor(element::ElementObject * elementObject)
	{
		if(elementObject == 0)
			return 0;

		return ClassDiagramController::RulesFor(elementObject->type());
	}

	/*!
	  This overloaded static method returns a rulechecker for the element type \c elementType. The best possible ruleCheckerType is returned.
	*/
	const IClassDiagramRules * ClassDiagramController::RulesFor(element::ElementType elementType)
	{
		if(_dd->_rules.contains(elementType))
			return _dd->_rules[elementType].get();
		else
			return _dd->_rules[element::Element].get();
	}
}
