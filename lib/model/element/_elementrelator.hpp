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

#ifndef ELEMENTRELATOR_HPP
#define ELEMENTRELATOR_HPP

#include "elementobject.hpp"
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

class ElementRelator
{
public:
	ElementRelator();

	std::vector<ElementObject *> findElementsRelatedTo(ElementObject * relatedElement);
	void updateElementObject(ElementObject * elementObject, ElementObject * oldRelatedElement);
	void removeElementObject(ElementObject * elementObject);
	void addElementObject(ElementObject * elementObject);

private:
	typedef boost::unordered_map<ElementObject*, boost::shared_ptr<std::vector<ElementObject*> > > relatedElements_map;
	relatedElements_map _relatedElements;

	void removeElementFromRelator(ElementObject * key, ElementObject * value);
	void addElementToRelator(ElementObject * key, ElementObject * value);
};

#endif // _ELEMENTRELATOR_HPP
