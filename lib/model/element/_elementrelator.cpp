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
#include "elementhelper.hpp"

namespace element
{
	QMap<ElementType, ElementRelator::Relator> ElementRelator::_Relators = QMap<ElementType, ElementRelator::Relator>();

	ElementRelator::ElementRelator()
	{
	}

	/*!
	  This helper method removes the combination attachedElement -> relatedElement from the list of
	  know links.
	*/
	void ElementRelator::removeElementFromRelator(ElementObject * relatedElement, ElementObject * attachedElement)
	{
		if(attachedElement == 0 || relatedElement == 0)
			return;

		if(_relatedElements.contains(attachedElement))
		{
			QList<ElementObject*> & relatedElements = *(_relatedElements[attachedElement]);
			relatedElements.removeAll(relatedElement);

			if(relatedElements.empty())
				_relatedElements.remove(attachedElement);
		}
	}

	/*!
	  This method returns a list of all the relatedElements to a certain attached Element.
	*/
	QList<ElementObject *> ElementRelator::findAllRelatedElementsTo(ElementObject * attachedElement)
	{
		if(_relatedElements.contains(attachedElement))
			return *(_relatedElements[attachedElement]);
		else
			return QList<ElementObject*>();
	}

	/*!
	  This method updates a the list. The link between oldAttachedElement and relatedElement is removed
	  and a new link between the new attachedElement and the relatedElement is stored. If oldAttachedElement
	  is zero, this method just add's the link from the new attachedElement to the relatedElement.
	*/
	void ElementRelator::update(ElementObject * relatedElement, ElementObject * oldAttachedElement)
	{
		if(oldAttachedElement != 0)
			removeElementFromRelator(relatedElement, oldAttachedElement);

		add(relatedElement);
	}

	/*!
	  This method remove a certain \c elementObject completely from the list of known links
	*/
	void ElementRelator::remove(ElementObject * elementObject)
	{
		// remove from key side (where elementObject is the attached element)
		relatedElements_map::iterator i = _relatedElements.find(elementObject);
		if(i != _relatedElements.end())
			_relatedElements.erase(i);

		// remove value side (where elementObject is the related element)
		removeElementFromRelator(elementObject, GetAttachedElement(elementObject));
	}

	/*!
	  This method first find the attachedElement for relatedElement. If that exists and is not zero,
	  it stores a new link from attachedElement tot relatedElement.
	*/
	void ElementRelator::add(ElementObject * relatedElement)
	{
		if(relatedElement == 0)
			return;

		ElementObject * attachedElement = GetAttachedElement(relatedElement);
		if(attachedElement == 0)
			return;

		boost::shared_ptr<QList<ElementObject*> > relatedList;

		if(_relatedElements.contains(attachedElement))
			relatedList = _relatedElements[attachedElement];
		else
		{
			relatedList = boost::shared_ptr<QList<ElementObject*> >(new QList<ElementObject*>);
			_relatedElements[attachedElement] = relatedList;
		}

		relatedList->push_back(relatedElement);
	}

	/*!
	  This method registers a \c relator for a certain elementtype. This Relator will be used to get the
	  attached element and to set the attached element of elements of this type.

	  \sa ElementRelator::Relator
	*/
	void ElementRelator::AddElementRelator(ElementType type, Relator relator)
	{
		_Relators[type] = relator;
	}

	/*!
	  This method returns the attached element for \c elementObject. If there is no Relator registered
	  for \c elementObject's type, it will return 0.

	  \sa ElementRelator::AddElementRelator
	*/
	ElementObject * ElementRelator::GetAttachedElement(ElementObject * elementObject)
	{
		if(elementObject == 0)
			return 0;

		if(!_Relators.contains(elementObject->type()))
			return 0;
		else
			return _Relators[elementObject->type()].getter(elementObject);
	}

	/*!
	  This method sets the attached element for \c elementObject. If there is no Relator registered
	  for \c elementObject's type, it will return false. If for some reason it is unable to set the attached
	  element, it will also return false. (e.g. \c newAttachedElements is the wrong type).

	  \sa ElementRelator::AddElementRelator
	*/
	bool ElementRelator::SetAttachedElement(ElementObject * elementObject, ElementObject * newAttachedElement)
	{
		if(elementObject == 0)
			return false;

		if(!_Relators.contains(elementObject->type()))
			return false;
		return _Relators[elementObject->type()].setter(elementObject, newAttachedElement);
	}
}
