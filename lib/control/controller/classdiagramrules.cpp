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

#include "classdiagramrules.hpp"
#include "methodobject.hpp"
#include "parameter.hpp"
#include "packageobject.hpp"
#include "classobject.hpp"
#include <QRegExp>

using namespace element;

namespace controller
{

	int ClassDiagramController::checkElement(element::ElementObject *element) const;



	bool DefaultClassDiagramRules::isValidName(const QString & newName) const
	{
		QRegExp checker("^[a-zA-Z_][a-zA-Z_0-9]*$");
		return checker.exactMatch(newName);
	}

	bool DefaultClassDiagramRules::isNameValidWithinParent(ElementObject * elementObject, ElementObject * parent, const QString & newName) const
	{
		if(parent != 0 && parent == elementObject)
			return false;

		QList<ElementObject*> siblings; // = controller()->diagram(
		if(siblings.size() == 0)
			return true;

		if(siblings.size() == 1 && siblings[0] == elementObject)
			return true;

		return false;
	}
//
//	bool ClassObjectDiagramRules::isParentRightContainerType(ElementObject * newParent) const
//	{
//		return newParent == 0 || element_castable<PackageObject>(newParent) || element_castable<ClassObject>(newParent);
//	}
//
//	bool PackageObjectDiagramRules::isParentRightContainerType(ElementObject * newParent) const
//	{
//		return newParent == 0 || element_castable<PackageObject>(newParent);
//	}
//
//
//	bool MethodObjectDiagramRules::isParentRightContainerType(ElementObject * newParent) const
//	{
//		return newParent == 0 || element_castable<PackageObject>(newParent);
//	}
//
//	bool MethodObjectDiagramRules::isNameValidWithinParent(ElementObject * elementObject, ElementObject * parent, const QString & newName) const
//	{
//		QList<ElementObject*> siblings = controller()->getElements(newName, parent);
//
//		if(siblings.size() == 0)
//			return true;
//
//		MethodObject * method = element_cast<MethodObject>(elementObject);
//		ParameterList list = (method == 0 ? ParameterList() : method->parameters());
//
//		for(int i = 0; i < siblings.size(); i++)
//		{
//			// all the elements in the list should be castable to a method
//			MethodObject * siblingObject = element_cast<MethodObject>(siblings[i]);
//
//			if(siblingObject == 0)
//				return false;
//
//			if(siblingObject == method)
//				continue;
//
//			// and the parameterlist should not be similar
//			if(controller()->AreParameterListsSimilar(list, siblingObject->parameters()))
//				return false;
//		}
//
//		return true;
//	}
//
//
//	bool OperationObjectDiagramRules::isParentRightContainerType(element::ElementObject * newParent) const
//	{
//		return newParent != 0 && element_castable<ClassObject>(newParent);
//	}
//
//	bool PropertyObjectDiagramRules::isParentRightContainerType(element::ElementObject * newParent) const
//	{
//		return newParent != 0 && element_castable<ClassObject>(newParent);
//	}
//
}

