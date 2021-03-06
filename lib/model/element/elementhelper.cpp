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

#include "elementhelper.hpp"
#include "classobject.hpp"
#include "methodobject.hpp"
#include "propertyobject.hpp"
#include "packageobject.hpp"
#include "operationobject.hpp"
#include "primitiveobject.hpp"
#include "_elementrelator.hpp"
#include <QVector>

namespace element
{
	/*!
	  This method returns a list of all the children from \c element whose name is equal to \c name. If name is empty, all the children are returned.
	*/
	QList<ElementObject*> findChildren(ElementObject * element, const QString & name)
	{

		typedef QVector<ElementObject*> elementvct;
		typedef QList<ElementObject*> elementlst;
		typedef elementvct::iterator elementvctit;

		if(!element)
			return elementlst();

		const QList<ElementObject * >	& children = element->children();
		elementvct transf(children.size());

		boost::function<bool (ElementObject*)> pred;
		if(name.isEmpty())
			pred = boost::bind(checkForValidElement<ElementObject>, _1);
		else
			pred = boost::bind(checkForValidNamedElement<ElementObject>, _1, boost::cref(name));


		elementvctit it = stf::copy_transformed_if(
				children.begin(),
				children.end(),
				transf.begin(),
				boost::bind<ElementObject*>(
						element_cast<ElementObject>,
						_1),
				pred
				);

		transf.erase(it, transf.end());
		return elementlst::fromVector(transf);
	}

	/*!
	  Creates an element of type \c type and sets the name.
	*/
	ElementObject * createElementObject(ElementType type, const QString & name)
	{
		switch(type)
		{
		case Element_Class:
			return new ClassObject(name);
		case Element_Method:
			return new MethodObject(name);
		case Element_Operation:
			return new OperationObject(name);
		case Element_Package:
			return new PackageObject(name);
		case Element_Primitive:
			return new PrimitiveObject(name);
		case Element_Property:
			return new PropertyObject(name);
		default:
			return 0;
		}
	}

	/*!
	  This method searches a list of elements for an element with a certain \c umlName.
	*/
	ElementObject * findUMLName(const QList<ElementObject*> & elements, const QString & umlName)
	{
		foreach(ElementObject * element, elements)
			if(element->umlName() == umlName)
				return element;

		return 0;
	}

	namespace
	{
		struct RecursiveNameFinder : public ElementFunctor
		{
			RecursiveNameFinder(const QString & name, QList<ElementObject *> * foundElements)
				: _name(name)
				, _foundElements(foundElements)
			{}

			QString _name;
			QList<ElementObject *> * _foundElements;

			virtual void operator()(ElementObject * elementObject)
			{
				if(elementObject->name() == _name)
					_foundElements->append(elementObject);
			}
		};
	}

	/*!
	  This method searches a list of elements for all the elements with a certain \c name. By default only in this list is searched,
	  but if \c isRecursive is set to true, the method searches recursively.
	*/
	QList<ElementObject *> findName(const QList<ElementObject*> & elements, const QString & name, bool isRecursive)
	{
		QList<ElementObject *> foundElements;

		if(!isRecursive)
		{
			foreach(ElementObject * curElement, elements)
				if(curElement->name() == name)
					foundElements << curElement;
		}
		else
		{
			RecursiveNameFinder r(name, &foundElements);
			recursivelyPerformOperation(elements, r);
		}

		return foundElements;
	}

	/*!
	  This method calls the operator() on each element recursively.
	*/
	void recursivelyPerformOperation(const QList<ElementObject*> & elements, ElementFunctor & f)
	{
		foreach(ElementObject * element, elements)
		{
			f(element);
			recursivelyPerformOperation(element->children(), f);
		}
	}

}
