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
#include <QMap>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace element
{
	/*!
	  This class is used as a helper to retain the connection between relatedElements and attachedElements.
	  A relatedElement is an element who contains a reference to an attachedElement in it's data type.

	  e.g.: A property is a relatedElement and it's attachedElement will be it's datatype. Similary we can
	  see that a method is a relatedElement and it's attachedElement is it's returntype.
	*/
	class ElementRelator
	{
	public:
		/*!
		  This struct contains the necessary information to go from an element to the attachedElement. The
		  getter returns the attachedElement and the setter will set a new value for the attachedElement.
		*/
		struct Relator
		{
			typedef boost::function<ElementObject* (ElementObject*)> GetAttachedElementFunction;
			typedef boost::function<bool (ElementObject*, ElementObject*)> SetAttachedElementFunction;

			Relator(GetAttachedElementFunction getter, SetAttachedElementFunction setter)
			{
				this->getter = getter;
				this->setter = setter;
			}

			Relator()
			{
			}

			GetAttachedElementFunction getter;
			SetAttachedElementFunction setter;
		};

		ElementRelator();

		QList<ElementObject *> findAllRelatedElementsTo(ElementObject * attachedElement);
		void update(ElementObject * relatedElement, ElementObject * oldAttachedElement);
		void remove(ElementObject * elementObject);
		void add(ElementObject * relatedElement);

		static void AddElementRelator(ElementType type, Relator relator);
		static ElementObject * GetAttachedElement(ElementObject * elementObject);
		static bool SetAttachedElement(ElementObject * elementObject, ElementObject * newAttachedElement);

	private:
		typedef QMap<ElementObject*, boost::shared_ptr<QList<ElementObject*> > > relatedElements_map;
		relatedElements_map _relatedElements;

		void removeElementFromRelator(ElementObject * relatedElement, ElementObject * attachedElement);

		static QMap<ElementType, Relator> _Relators;
	};
}
#endif // _ELEMENTRELATOR_HPP
