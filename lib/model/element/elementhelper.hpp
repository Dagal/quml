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

#ifndef ELEMENTHELPER_HPP
#define ELEMENTHELPER_HPP

#include "elementobject.hpp"
#include "algorithm.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include "singleton.hpp"

namespace element
{
/*!
  This method checks if a certain element is castable to certain type. This is based on the type of an element.

  \sa ElementObject::type, \sa ElementObject::elementtype, \sa element_cast
*/
	template <typename ElementClass> bool element_castable(const ElementObject * element)
	{
		if(!element) return false;

		return ((element->type() & ElementClass::elementtype) == ElementClass::elementtype);
	}

	template <typename ElementClassA, typename ElementClassB> bool isACastableToB(const ElementClassA * /*elementA*/ = 0, const ElementClassB * /*elementB*/ = 0)
	{
		return (ElementClassA::elementtype & ElementClassB::elementtype) == ElementClassB::elementtype;
	}

	/*!
  This method tries to cast an element to a certain type. If unable it returns a zero value.

  \sa ElementObject::type, \sa ElementObject::elementtype
*/
	template <typename ElementClass> ElementClass * element_cast(ElementObject * element)
	{
		if(!element_castable<ElementClass>(element))
			return 0;

		return static_cast<ElementClass*>(element);
	}

	/*!
  This method tries to cast a const element to a certain type. If unable it returns a zero value.

  \sa ElementObject::type, \sa ElementObject::elementtype
*/
	template <typename ElementClass> const ElementClass * element_cast(const ElementObject * element)
	{
		if(!element_castable<ElementClass>(element))
			return 0;

		return static_cast<const ElementClass*>(element);
	}

	template <typename T> bool checkForValidNamedElement(T * element, const QString & name)
	{
		return (element && element->name() == name);
	}
	template <typename T> bool checkForValidElement(T * element)
	{
		return (element);
	}

	/*!
  This method returns a list of all the child from \c element that can be casted to \c ElementClass. If \c name is defined, the names of the children
  should also match.
*/
	template <typename ElementClass> QList<ElementClass*> findChildren(ElementObject * element, const QString & name = QString())
	{
		typedef QList<ElementClass*> elementvct;
		typedef typename elementvct::iterator elementvctit;

		if(element == 0)
			return elementvct();

		const QList<ElementObject * > & children = element->children();
		elementvct transf(children.size());

		boost::function<bool (ElementClass*)> pred;
		if(name.isEmpty())
			pred = boost::bind(checkForValidElement<ElementClass>, _1);
		else
			pred = boost::bind(checkForValidNamedElement<ElementClass>, _1, boost::cref(name));


		elementvctit it = stf::copy_transformed_if(
				children.begin(),
				children.end(),
				transf.begin(),
				boost::bind<ElementClass*>(
						element_cast<ElementClass>,
						_1),
				pred
				);

		transf.erase(it, transf.end());
		return transf;
	}

	QList<ElementObject*> findChildren(ElementObject * element, const QString & name = QString());

	ElementObject * createElementObject(ElementType type, const QString & name);
	ElementObject * findUMLName(const QList<ElementObject*> & elements, const QString & umlName);
	QList<ElementObject *> findName(const QList<ElementObject*> & elements, const QString & name, bool isRecursive = false);

	struct ElementFunctor
	{
		virtual void operator()(ElementObject * element) = 0;
	};

	void recursivelyPerformOperation(const QList<ElementObject*> & elements, ElementFunctor & f);
}

#endif // ELEMENTHELPER_HPP
