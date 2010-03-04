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

ClassDiagramController::ClassDiagramController(UMLDiagram * diagram)
	: _dd(new ClassDiagramControllerPrivate(diagram))
{
}

UMLDiagram * ClassDiagramController::diagram() const
{
	return _dd->_diagram;
}

Error ClassDiagramController::deleteElement(const std::string & qualifiedElementName)
{
	ElementObject * element = getElement(qualifiedElementName);
	if(!element) return Error_ElementUndefined;

	element->setUMLDiagram(0);
	element->setParent(0);
	delete element;

	return Error_NoError;
}

Error ClassDiagramController::renameElement(const std::string & qualifiedElementName, const std::string & newName)
{
	ElementObject * element = getElement(qualifiedElementName);
	if(!element) return Error_ElementUndefined;

	if(newName.empty()) return Error_ElementNameEmpty;

	if(element->parent())
		if(!findChildren(element->parent(), newName).empty()) return Error_ElementNameAlreadyUsed;
	else


}

Error ClassDiagramController::createClass(const std::string & className, const std::string & qualifiedParentName);
Error ClassDiagramController::moveClass(const std::string & classQualifiedName, const std::string & newParentQualifiedName);

// find methods
ElementObject * getElement(const std::string & qualifiedName) const;

