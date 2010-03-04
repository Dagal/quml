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

#ifndef METHODOBJECT_HPP
#define METHODOBJECT_HPP

#include "elementobject.hpp"
#include "datatypeobject.hpp"

class MethodObject : public ElementObject
{
	class MethodObjectPrivate;

public:
	enum { elementtype = Element_Method };

	MethodObject(const std::string & name);
	~MethodObject() {}


	DatatypeObject * returnType() const;
	void setReturnType(DatatypeObject * returnType);
	const std::vector<ParameterObject*> & parameters() const;
	ParameterObject * parameterAt(unsigned int position) const;

	virtual std::string umlName() const;
	virtual ElementType type() const { return Element_Method; }

private:
	virtual void onChildAdded(ElementObject * child);
	virtual void onChildRemoved(ElementObject * child);

private:
	boost::shared_ptr<MethodObjectPrivate> _dd;
};

#endif // METHODOBJECT_HPP
