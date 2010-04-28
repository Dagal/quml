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

#include "methodobject.hpp"
#include "_methodobject.hpp"
#include "datatypeobject.hpp"
#include "elementhelper.hpp"
#include "_umldiagram.hpp"

namespace element
{
	MethodObject::MethodObject(const QString & name)
		: ElementObject(name), _dd(new MethodObjectPrivate)
	{
	}

	DatatypeObject * MethodObject::returnType() const
	{
		return _dd->_returnType;
	}
	void MethodObject::setReturnType(DatatypeObject * returnType)
	{
		if(returnType != 0 && returnType->umlDiagram() != umlDiagram())
			returnType = 0;

		if(_dd->_returnType == returnType)
			return;

		DatatypeObject * oldData = _dd->_returnType;
		_dd->_returnType = returnType;
		attachedElementChanged(oldData);
	}

	const ParameterList & MethodObject::parameters() const
	{
		return _dd->_parameters;
	}

	Parameter MethodObject::parameterAt(unsigned int position) const
	{
		return _dd->_parameters[position];
	}

	QString MethodObject::umlName() const
	{
		QString retVal = name() + "(";

		for(int i = 0; i < parameters().size(); i++)
		{
			retVal += _dd->parameterToUmlNotation(parameterAt(i));
			if(i != parameters().size()-1)
				retVal += ", ";
		}

		retVal += "): " + returnType()->umlName();

		return retVal;
	}

	/*!
	  \todo Implement this method
	*/
	QString MethodObject::MethodObjectPrivate::parameterToUmlNotation(const Parameter & parameter)
	{
		return QString("to be implemented: ");
	}

	void MethodObject::setParameterList(const ParameterList & list)
	{
		_dd->_parameters = list;
	}
}
