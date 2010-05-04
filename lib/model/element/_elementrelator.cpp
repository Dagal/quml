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
	//QMap<ElementType, ElementRelator::Relator> ElementRelator::_Relators = QMap<ElementType, ElementRelator::Relator>();

	ElementRelator::ElementRelator()
	{
	}

	/*!
	  This method returns a list of all the relatedElements to a certain attached Element.
	*/
	QList<ElementObject *> ElementRelator::findAllRelatedElementsTo(ElementObject * attachedElement)
	{
		if(!_relatedElements.contains(attachedElement))
			return QList<ElementObject*>();

		QList<ElementObject*> returnList;
		QList<RelatedElementDetails> & relatedList = _relatedElements[attachedElement];
		for(int i = 0; i < relatedList.size(); i++)
			if(relatedList[i]._relatedElement != 0)
				returnList << relatedList[i]._relatedElement;

		return returnList;
	}

	/*!
	  This method updates a the list. The link between oldAttachedElement and relatedElement is removed
	  and a new link between the new attachedElement and the relatedElement is stored. If oldAttachedElement
	  is zero, this method just add's the link from the new attachedElement to the relatedElement.
	*/
	void ElementRelator::update(ElementObject * relatedElement, int position, ElementObject * oldAttachedElement)
	{
		RelatedElementDetails detail(relatedElement, position);

		if(oldAttachedElement != 0)
			removeRelation(oldAttachedElement, detail);

		addRelation(relatedElement->getAttachedElementAt(position), detail);
	}

	/*!
	  This method remove a certain \c elementObject completely from the list of known relations, as a relatedElement but also as a attachedElement
	*/
	void ElementRelator::remove(ElementObject * elementObject)
	{
		if(elementObject == 0)
			return;

		// remove from key side (where elementObject is the attached element)
		_relatedElements.remove(elementObject);

		// remove value side (where elementObject is the related element)
		for(int i = 0; i < elementObject->attachedElementCount(); i++)
		{
			ElementObject * attachedElement = elementObject->getAttachedElementAt(i);
			if(_relatedElements.contains(attachedElement))
			{
				QList<RelatedElementDetails> & lst = _relatedElements[attachedElement];
				QList<RelatedElementDetails>::iterator i = std::find(
						lst.begin(),
						lst.end(),
						elementObject
						);

				if(i != lst.end())
					lst.erase(i);
			}
		}
	}

	/*!
	  This is a helper method to add a certain relation from attachedElement to RelatedElement. If the attachedElement or the RelatedElement
	  is zero, no adding is performed.
	*/
	void ElementRelator::addRelation(ElementObject * attachedElement, RelatedElementDetails elementDetails)
	{
		if(attachedElement == 0 || elementDetails._relatedElement == 0)
			return;

		QList<RelatedElementDetails> & lst = _relatedElements[attachedElement];
		lst.push_back(elementDetails);
	}

	/*!
	  This ia helper method to remove a certain relation from attachedElement to RelatedElement.
	*/
	void ElementRelator::removeRelation(ElementObject * attachedElement, RelatedElementDetails details)
	{
		if(attachedElement == 0 || details._relatedElement == 0)
			return;

		if(_relatedElements.contains(attachedElement))
			_relatedElements[attachedElement].removeAll(details);
	}

	/*!
	  This method first find the attachedElement for relatedElement. If that exists and is not zero,
	  it stores a new link from attachedElement tot relatedElement.
	*/
	void ElementRelator::add(ElementObject * relatedElement)
	{
		if(relatedElement == 0 || relatedElement->attachedElementCount() == 0)
			return;

		for(int i = 0 ; i < relatedElement->attachedElementCount(); i++)
		{
			ElementObject * attachedElement = relatedElement->getAttachedElementAt(i);
			addRelation(attachedElement, RelatedElementDetails(relatedElement, i));
		}
	}

	/*!
	  This method returns a list of all the RelatedElementDetails where this element is the attachedElement.
	*/
	QList<ElementRelator::RelatedElementDetails> ElementRelator::findAllRelatedElementDetailsTo(ElementObject * attachedElement)
	{
		if(_relatedElements.contains(attachedElement))
			return _relatedElements[attachedElement];
		else
			return QList<RelatedElementDetails>();
	}

}
