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
#include <boost/shared_ptr.hpp>

struct UMLDiagram::UMLDiagramPrivate
{
	typedef std::vector<boost::shared_ptr<ElementObject> > element_vector;
	typedef boost::unordered_map<std::string, boost::shared_ptr<std::vector<ElementObject*> > > relatedElements_map;

	UMLDiagramPrivate()
	{
	}

	element_vector _elements;
	relatedElements_map _relatedElements;

	void addNewElement(boost::shared_ptr<ElementObject> elementObject);
	boost::shared_ptr<ElementObject> removeElement(const std::string & qualifiedName);

	void updateRelatedValueElement(ElementObject * elementObject, const std::string & oldQualifiedName, const std::string & newQualifiedName);
	void updateRelatedKeyElement(const std::string & oldQualifiedName, const std::string & newQualifiedName);
	element_vector::iterator findElement(const std::string & qualifiedName);

private:
	void resortElements();
	void removeRelatedValueElement(const std::string & oldQualifiedName, ElementObject * element);
	void addRelatedValueElement(const std::string & newQualifiedName, ElementObject * element);
};

#endif // _UMLDIAGRAM_HPP
