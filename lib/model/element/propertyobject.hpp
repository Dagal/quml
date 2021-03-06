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

#ifndef PROPERTYOBJECT_HPP
#define PROPERTYOBJECT_HPP

#include "elementobject.hpp"
#include "parameter.hpp"
#include "visibility.hpp"

namespace element
{
	/*!
	  \brief The PropertyObject class contains all the information to store the UML model of a property.
	*/
	class PropertyObject : public ElementObject
	{
		class PropertyObjectPrivate;

	public:
		enum { elementtype = Element_Property };

		PropertyObject(const QString & name);

		VisibilityType visibility() const;
		DatatypeObject * datatype() const;
		const QString & defaultValue() const;
		void setVisibility(VisibilityType type);
		void setDatatype(DatatypeObject * datatype);
		void setDefaultValue(const QString & defaultValue);

		// overridden member functions
		virtual int attachedElementCount() const { return 1; }
		virtual ElementObject * getAttachedElementAt(int position) const;
		virtual void setAttachedElementAt(int position, ElementObject * attachedElement);
		virtual ElementType type() const { return Element_Property; }
		virtual QString umlName() const;

	private:
		boost::shared_ptr<PropertyObjectPrivate> _dd;
	};
}

#endif // PROPERTYOBJECT_HPP
