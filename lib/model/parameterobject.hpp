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

#include "elementobject.hpp"

class ParameterObject : public ElementObject
{
	class ParameterObjectPrivate;

public:
	enum { elementtype = Element_Parameter };

	ParameterObject();

	const std::string & datatype() const;
	const std::string & defaultValue() const;
	void setDatatype(const std::string & datatype);
	void setDefaultValue(const std::string & defaultValue);

	virtual ElementType type() const { return Element_Parameter; }

	virtual std::string umlName() const;

private:
	boost::shared_ptr<ParameterObjectPrivate> _dd;
};

#endif // PARAMETEROBJECT_HPP
