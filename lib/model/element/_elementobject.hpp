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

#ifndef P_ELEMENTOBJECT_HPP
#define P_ELEMENTOBJECT_HPP

#include "elementobject.hpp"
#include "elementhelper.hpp"

namespace element
{
	/*!
	  \internal
	  \class element::ElementObject::ElementObjectPrivate
	  \brief The ElementObjectPrivate class is the PIMPL-implementation for ElementObject
	*/
	struct ElementObject::ElementObjectPrivate
	{
		ElementObjectPrivate(ElementObject * elementObject)
			: _parent(0)
			, _diagram(0)
			, _element(elementObject)
		{
		}

		QString _name;
		ElementObject * _parent;
		UMLDiagram * _diagram;
		ElementObject * _element;
		QList<ElementObject *> _children;

		void detachFromParentAndDiagram();
		void INT_detachFromUMLAndParent();
		void INT_attachToUMLAndParent(ElementObject * parent);
		void INT_detachFromParent();
		void INT_attachToParent(ElementObject * parent);
		void INT_attachToUMLDiagram(UMLDiagram * diagram);
	};
}

#endif // P_ELEMENTOBJECT_HPP
