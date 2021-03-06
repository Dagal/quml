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
#include "parameter.hpp"

namespace element
{
	/*!
	  \brief The MethodObject class contains all the information to store the UML model of a method.
	*/
	class MethodObject : public ElementObject
	{
		class MethodObjectPrivate;

	public:
		enum { elementtype = Element_Method };

		MethodObject(const QString & name);
		~MethodObject() {}


		DatatypeObject * returnType() const;
		void setReturnType(DatatypeObject * returnType);
		const ParameterList & parameters() const;
		const Parameter & parameterAt(unsigned int position) const;

		void setParameter(int position, const Parameter & parameter);
		void insertParameter(int position, const Parameter & parameter);
		void addParameter(const Parameter & parameter);
		void removeParameter(int position);

		void setParameterList(const ParameterList & list);

		// overridden member functions
		virtual int attachedElementCount() const;
		virtual ElementObject * getAttachedElementAt(int position) const;
		virtual void setAttachedElementAt(int position, ElementObject * attachedElement);
		virtual QString umlName() const;
		virtual ElementType type() const { return Element_Method; }

	private:
		boost::shared_ptr<MethodObjectPrivate> _dd;
	};
}

#endif // METHODOBJECT_HPP
