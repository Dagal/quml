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
#include <boost/unordered_map.hpp>

struct UMLDiagram::UMLDiagramPrivate
{
	typedef std::vector<ElementObject *> elementvct;

	UMLDiagramPrivate()
		: _diagram(0), _emptyPackage(0)
	{
	}

	void initialise(UMLDiagram * umldiagram);

	void attachElementObject(ElementObject * element);
	void detachElementObject(const std::string & qualifiedName);
	void changeElementName(const std::string & oldName, const std::string & newName);

	elementvct::const_iterator findInElements(const std::string & name) const;
	elementvct::iterator findInElements(const std::string & name);
	void emptyLocation(const std::string & name);
	void resortElements();

	elementvct _elements;
	UMLDiagram * _diagram;
	PackageObject * _emptyPackage;
};

#endif // _UMLDIAGRAM_HPP
