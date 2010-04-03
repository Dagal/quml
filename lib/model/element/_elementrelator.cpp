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

#include "_elementrelator.hpp"
#include "methodobject.hpp"
#include "parameterobject.hpp"
#include "elementhelper.hpp"

namespace element
{
	ElementRelator::ElementRelator()
	{
	}

	void ElementRelator::removeElementFromRelator(ElementObject * key, ElementObject * value)
	{
		if(key == 0 || value == 0)
			return;

		// find the key
		relatedElements_map::iterator i = _relatedElements.find(key);
		if(i != _relatedElements.end())
		{
			// find the value in the attached elements
			std::vector<ElementObject*> & vct = *(i->second);
			std::vector<ElementObject*>::iterator j = std::find(
					vct.begin(),
					vct.end(),
					value
					);

			// and erase it from the vector
			if(j != vct.end())
				vct.erase(j);

			// if empty, delete this key
			if(vct.empty())
				_relatedElements.erase(i);
		}
	}

	void ElementRelator::addElementToRelator(ElementObject * key, ElementObject * value)
	{
		if(key == 0 || value == 0)
			return;

		// find the key
		relatedElements_map::iterator i = _relatedElements.find(key);

		// not found?
		if(i == _relatedElements.end())
		{
			// create new vector and store that for value with key
			boost::shared_ptr<std::vector<ElementObject*> > ptr(new std::vector<ElementObject*>);
			_relatedElements[key] = ptr;
			ptr->push_back(value);
		}
		else
		{
			// add it to the related elements array
			i->second->push_back(value);
		}
	}

	std::vector<ElementObject *> ElementRelator::findElementsRelatedTo(ElementObject * relatedElement)
	{
		relatedElements_map::iterator i = _relatedElements.find(relatedElement);
		if(i == _relatedElements.end())
			return std::vector<ElementObject*>();
		else
			return *(i->second);
	}

	void ElementRelator::updateElementObject(ElementObject * elementObject, ElementObject * oldRelatedElement)
	{
		if(oldRelatedElement == 0)
		{
			addElementObject(elementObject);
			return;
		}

		removeElementFromRelator(oldRelatedElement, elementObject);
		addElementToRelator(findRelatedElement(elementObject), oldRelatedElement);
	}

	void ElementRelator::removeElementObject(ElementObject * elementObject)
	{
		// remove from key side
		relatedElements_map::iterator i = _relatedElements.find(elementObject);
		if(i != _relatedElements.end())
			_relatedElements.erase(i);

		// remove value side
		removeElementFromRelator(findRelatedElement(elementObject), elementObject);
	}

	void ElementRelator::addElementObject(ElementObject * elementObject)
	{
		addElementToRelator(findRelatedElement(elementObject), elementObject);
	}
}
