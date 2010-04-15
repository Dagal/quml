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

#ifndef ELEMENTOBJECT_HPP
#define ELEMENTOBJECT_HPP

#include "element.hpp"
#include <QString>
#include <QList>


namespace element
{
	/*!
	  \brief The ElementObject class is the base class for all UML objects

	  ElementObject implements the composition pattern. So different child ElementObjects can be added to a parent ElementObject. If the parent element gets
	  deleted, it will automatically delete all child objects.
	*/
	class ElementObject
	{
		friend class UMLDiagram;
		class ElementObjectPrivate;

	public:
		// enum hack (for template functions)
		enum { elementtype = Element /*!<  Reimplement this enum value in the inheriting classes so ensure casting to different element types is possible. \sa element_cast*/};

		// constructor & destructor
		ElementObject(const QString & name);
		virtual ~ElementObject() = 0;

		// simple getters
		const QString & name() const;
		ElementObject * parent() const;
		UMLDiagram * umlDiagram() const;
		const QList<ElementObject *> & children() const;
		QList<ElementObject*> ancestors() const;

		// virtual functions
		virtual QString qualifiedName() const;
		virtual QString umlName() const;

		/*!
		Reimplement this abstract to return the right ElementType. This is necessary for casting different element types.
		\sa element_cast
		*/
		virtual ElementType type() const  = 0;

		// setters
		void setName(const QString & name);
		void setParent(ElementObject * parent);
		void setUMLDiagram(UMLDiagram * diagram);

	private:
		virtual void onChildAdded(ElementObject * child);
		virtual void onChildRemoved(ElementObject * child);

	protected:
		void relatedElementChanged(ElementObject * oldRelatedElement);

	private:
		boost::shared_ptr<ElementObjectPrivate> _dd;
	};

}

#endif // ELEMENTOBJECT_HPP
