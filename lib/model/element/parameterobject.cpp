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

#include "parameterobject.hpp"
#include "_parameterobject.hpp"
#include "datatypeobject.hpp"
#include "_umldiagram.hpp"
#include <sstream>

ParameterObject::ParameterObject()
	: ElementObject(), _dd(new ParameterObjectPrivate)
{
}

const std::string & ParameterObject::datatype() const
{
	return _dd->_datatype;
}

const string & ParameterObject::defaultValue() const
{
	return _dd->_defaultValue;
}

void ParameterObject::setDatatype(const std::string & datatype)
{
	if(_dd->_datatype == datatype)
		return;

	relatedElementAboutToChange();
	_dd->_datatype = datatype;
	relatedElementChanged();
}

void ParameterObject::setDefaultValue(const string & defaultValue)
{
	_dd->_defaultValue = defaultValue;
}

std::string ParameterObject::umlName() const
{
	std::stringstream s;

	s << name() << ": " << datatype();

	return s.str();
}
