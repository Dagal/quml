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
		attachedElementChanged(0, oldData);
	}

	const ParameterList & MethodObject::parameters() const
	{
		return _dd->_parameters;
	}

	const Parameter & MethodObject::parameterAt(unsigned int position) const
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

		if(returnType())
			retVal += "): " + returnType()->umlName();

		return retVal;
	}

	/*!
	  \todo Implement this method
	*/
	QString MethodObject::MethodObjectPrivate::parameterToUmlNotation(const Parameter & parameter)
	{
		QString retVal = parameter.name();
		if(parameter.datatype())
			retVal += ": " + parameter.datatype()->qualifiedName();

		return retVal;
	}

	/*!
	  This method gets rid of the complete parameterList and sets its to \c list.
	*/
	void MethodObject::setParameterList(const ParameterList & list)
	{
		// perform cleanup
		while(parameters().size() != 0)
			removeParameter(parameters().size()-1);

		// add the new value
		for(int i = 0; i < list.size(); i++)
			addParameter(list[i]);
	}

	/*!
	  This method sets the parameter at \c position to value \c parameter. If no parameter exists at the position (position < 0 || position >= parameterCount)
	  the parameter is added to the end of the parameterList.

	  \sa addParameter, insertParameter
	*/
	void MethodObject::setParameter(int position, const Parameter & parameter)
	{
		if(position < 0 || position >= parameters().size())
		{
			addParameter(parameter);
			return;
		}

		DatatypeObject * oldDatatype = _dd->_parameters[position].datatype();

		_dd->_parameters[position] = parameter;
		attachedElementChanged(position+1, oldDatatype);
	}

	/*!
	  This method insert a parameter at the given \c position with the value \c parameter. If the position is invalid, the parameter is inserted at the end

	  \sa addParameter
	*/
	void MethodObject::insertParameter(int position, const Parameter & parameter)
	{
		if(position < 0 || position >= parameters().size())
		{
			addParameter(parameter);
			return;
		}

		// insert the parameter
		_dd->_parameters.insert(position, parameter);

		// send the message tot the relator
		for(int i = position; i < parameters().size(); i++)
		{
			ElementObject * oldDatatype = (position == parameters().size()-1 ? 0 : parameters()[position+1].datatype());
			attachedElementChanged(position+1, oldDatatype);
		}
	}

	/*!
	  This methods add a parameter to the end of the parameterlist. This parameter is the last parameter in the method.
	*/
	void MethodObject::addParameter(const Parameter & parameter)
	{
		_dd->_parameters.push_back(parameter);
		attachedElementChanged(parameters().size(), 0);
	}

	/*!
	  THis function removes the parameter at the position.
	*/
	void MethodObject::removeParameter(int position)
	{
		if(position < 0 || position >= parameters().size())
			return;

		ElementObject * oldDatatype = parameters()[position].datatype();

		// remove the parameter
		_dd->_parameters.removeAt(position);

		// send the message to the relator
		for(int i = position; i < parameters().size(); i++)
		{
			attachedElementChanged(position+1, oldDatatype);
			oldDatatype = parameters()[position].datatype();
		}

		attachedElementChanged(position+1, oldDatatype);
	}

	int MethodObject::attachedElementCount() const
	{
		return parameters().size() + 1;

	}

	ElementObject * MethodObject::getAttachedElementAt(int position) const
	{
		if(position > parameters().size() || position < 0)
			return 0;

		if(position == 0)
			return returnType();
		else
			return parameters()[position-1].datatype();
	}

	void MethodObject::setAttachedElementAt(int position, ElementObject * attachedElement)
	{
		if(position > parameters().size() || position < 0)
			return;

		DatatypeObject * datatypeObject = element_cast<DatatypeObject>(attachedElement);

		if(position == 0)
			setReturnType(datatypeObject);
		else
		{
			Parameter p = _dd->_parameters[position-1];
			p.setDatatype(datatypeObject);
			setParameter(position-1, p);
		}
	}
}
