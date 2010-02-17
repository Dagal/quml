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
#include "parameterobject.hpp"

MethodObject::MethodObject(ElementObject * parent)
	: ElementObject(parent), _dd(new MethodObjectPrivate)
{
}

DatatypeObject * MethodObject::returnType() const
{
	return _dd->_returnType;
}
void MethodObject::setReturnType(DatatypeObject * returnType)
{
	_dd->_returnType = returnType;
}

ParameterObject * MethodObject::removeParameter(const std::string & name)
{
	ParameterObject * param = _dd->_parameters.findElement(name);
	if(!param)
		return 0;

	param->setParent(0);
	return param;
}

const std::vector<ParameterObject*> & MethodObject::parameters() const
{
	return _dd->_parameters.vector();
}

ParameterObject * MethodObject::parameterAt(unsigned int position) const
{
	return _dd->_parameters.getElement(position);
}

virtual std::string MethodObject::umlName() const
{

}

