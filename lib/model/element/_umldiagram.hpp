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

#ifndef _UMLDIAGRAM_HPP
#define _UMLDIAGRAM_HPP

#include "umldiagram.hpp"
#include "_elementrelator.hpp"

struct AttachedElement
{
	ElementObject * elementObject;
	ElementObject * relatedElement;
};

struct UMLDiagram::UMLDiagramPrivate
{
	typedef std::vector<ElementObject* > element_vector;

	UMLDiagramPrivate()
	{
	}

	element_vector _elements;
	UMLDiagram * _diagram;
	ElementRelator _elementRelator;

	void attachElement(ElementObject * elementObject);
	void detachElement(ElementObject * elementObject);
	void resortElements();
	std::vector<ElementObject*>::iterator findElement(const std::string & qualifiedName);
	std::vector<ElementObject*>::iterator findElement(ElementObject * elementObject);


private:
	void INT_recursiveAttachElement(ElementObject * elementObject);
	void INT_recursiveDetachElement(ElementObject * elementObject);
	void INT_recursiveRemoveStrangeAttachedElements(ElementObject * elementObject);
	void INT_removeElementFromLists(ElementObject * elementObject);
	void INT_addElementToLists(ElementObject * elementObject);
};

#endif // _UMLDIAGRAM_HPP
