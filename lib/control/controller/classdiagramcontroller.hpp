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

#ifndef CLASSDIAGRAMCONTROLLER_HPP
#define CLASSDIAGRAMCONTROLLER_HPP

#include "elementhelper.hpp"
#include "action.hpp"
#include "macro.hpp"
#include "notifier.hpp"
#include "parameter.hpp"
#include <vector>

class ClassDiagramController
{
	class ClassDiagramControllerPrivate;

public:
	ClassDiagramController(element::UMLDiagram * diagram);

	// simple getters
	element::UMLDiagram * diagram() const;

	// element methods
	Error detachElement(const QString & qualifiedElementName, element::ElementObject ** elementObject = 0);
	Error renameElement(const QString & qualifiedElementName, const QString & newName);
	Error moveElement(const QString & qualifiedElementName, const QString & newParentQualifiedName);
	Error moveAndRenameElement(const QString & qualifiedElementName, const QString & newParentQualifiedName, const QString & newName);

	// class methods
	Error createClass(const QString & className, const QString & qualifiedParentName, element::ClassObject ** classObject = 0);

	// package methods
	Error createPackage(const QString & packageName, const QString & qualifiedParentName, element::PackageObject ** packageObject = 0);

	// find/get methods
	element::ElementObject * getElement(const QString & qualifiedName) const;
	template <typename T> T * getElement(const QString & qualifiedName) const;
	QList<element::ElementObject *> getElements(const QString & name, const QString & parentQualifiedName) const;
	QList<element::ElementObject *> getElements(const QString & name, element::ElementObject * parentObject) const;

	// helper methods
	Error checkParameterList(const element::ParameterList & list) const;
	bool checkForSimilarParameterLists(const element::ParameterList & listA, const element::ParameterList & listB) const;

	const Notifier<Action> & actionListener() const { return _actionListener; }

protected:
	bool checkNameAgainstSiblings(element::ElementObject * element, const QString & newName, element::ElementObject * parentObject) const;
	bool checkNameAgainstSiblings(element::ElementType type, const QString & newName, element::ElementObject * parentObject) const;

private:
	boost::shared_ptr<ClassDiagramControllerPrivate> _dd;
	Notifier<Action> _actionListener;
};

template <typename T> T * ClassDiagramController::getElement(const QString & qualifiedName) const
{
	return element::element_cast<T>(getElement(qualifiedName));
}

#endif // CLASSDIAGRAMCONTROLLER_HPP
