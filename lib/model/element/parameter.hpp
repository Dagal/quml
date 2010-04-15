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

#ifndef PARAMETEROBJECT_HPP
#define PARAMETEROBJECT_HPP

#include "datatypeobject.hpp"

namespace element
{
	typedef QList<Parameter> parameterList;

	/*!
	  \brief The ParameterObject class contains all the information to store the UML model of a parameter for a method, it is a simple container
	*/
	class Parameter
	{
	public:
		Parameter(const QString & name)
			: _name(name)
			,_datatype(0)
		{}

		DatatypeObject * datatype() const					{ return _datatype; }
		const QString & defaultValue() const				{ return _defaultValue; }
		const QString & name() const						{ return _name; }
		void setDatatype(DatatypeObject * datatype)			{ _datatype = datatype; }
		void setDefaultValue(const QString & defaultValue)	{ _defaultValue = defaultValue; }
		void setName(const QString & name)					{ _name = name; }

	private:
		QString _name;
		DatatypeObject * _datatype;
		QString _defaultValue;
	};
}

#endif // PARAMETEROBJECT_HPP
