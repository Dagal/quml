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
#include "error.hpp"
#include <vector>

class UMLDiagram;
class ElementObject;

class ClassDiagramController
{
	class ClassDiagramControllerPrivate;

public:
	ClassDiagramController(UMLDiagram * diagram);

	// simple getters
	UMLDiagram * diagram() const;

	// element methods
	Error detachElement(const std::string & qualifiedElementName, ElementObject ** elementObject = 0);
	Error renameElement(const std::string & qualifiedElementName, const std::string & newName);

	// class methods
	Error createClass(const std::string & className, const std::string & qualifiedParentName, ClassObject ** elementObject = 0);
	Error moveClass(const std::string & classQualifiedName, const std::string & newParentQualifiedName);

	// package methods
	Error createPackage(const std::string & packageName, const std::string & newParentQualifiedName, PackageObject ** elementObject = 0);
	Error movePackage(const std::string & packageQualifiedName, const std::string & newParentQualifiedName);

	// find/get methods
	ElementObject * getElement(const std::string & qualifiedName) const;
	template <typename T> T * getElement(const std::string & qualifiedName) const;
	std::vector<ElementObject *> getElements(const std::string & name, const std::string & parentQualifiedName) const;
	std::vector<ElementObject *> getElements(const std::string & name, ElementObject * parentObject) const;

protected:
	bool checkNameAgainstSiblings(ElementObject * element, const std::string & newName, ElementObject * parentObject) const;

private:
	boost::shared_ptr<ClassDiagramControllerPrivate> _dd;
};

template <typename T> T * ClassDiagramController::getElement(const std::string & qualifiedName) const
{
	return element_cast<T>(getElement(qualifiedName));
}

#endif // CLASSDIAGRAMCONTROLLER_HPP
