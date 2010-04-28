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

#include "propertyobject.hpp"
#include "_propertyobject.hpp"
#include "elementhelper.hpp"

namespace element
{
	PropertyObject::PropertyObject(const QString & name)
		: ElementObject(name), _dd(new PropertyObjectPrivate)
	{
	}

	void PropertyObject::setVisibility(VisibilityType type)
	{
		_dd->_visibility.setType(type);
	}

	VisibilityType PropertyObject::visibility() const
	{
		return _dd->_visibility.type();
	}

	DatatypeObject * PropertyObject::datatype() const
	{
		return _dd->_datatype;
	}

	const QString & PropertyObject::defaultValue() const
	{
		return _dd->_defaultValue;
	}

	void PropertyObject::setDatatype(DatatypeObject * datatype)
	{
		if(datatype != 0 && datatype->umlDiagram() != umlDiagram())
			datatype = 0;

		if(_dd->_datatype == datatype)
			return;


		DatatypeObject * oldData = _dd->_datatype;
		_dd->_datatype = datatype;
		attachedElementChanged(0, oldData);
	}

	void PropertyObject::setDefaultValue(const QString & defaultValue)
	{
		_dd->_defaultValue = defaultValue;
	}

	QString PropertyObject::umlName() const
	{
		if(datatype())
			return name() + ": " + datatype()->umlName();
		else
			return name() + ": void";
	}

	ElementObject * PropertyObject::getAttachedElementAt(int position) const
	{
		if(position == 0)
			return datatype();
		else
			return 0;
	}

	void PropertyObject::setAttachedElementAt(int position, ElementObject * attachedElement)
	{
		DatatypeObject * object = element_cast<DatatypeObject>(attachedElement);

		if(position == 0 )
			setDatatype(object);
	}
}

