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

#ifndef ELEMENTHELPER_HPP
#define ELEMENTHELPER_HPP

#include "elementobject.hpp"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/if.hpp>
#include <iostream>

template <typename ElementClass> ElementClass * element_cast(ElementObject * element)
{
	if(!element) return 0;

	if((element->type() && ElementClass::elementtype) != ElementClass::elementtype)
		return 0;

	return static_cast<ElementClass*>(element);
}

template <typename ElementClass> const ElementClass * element_cast(const ElementObject * element)
{
	if(!element) return 0;

	if((element->type() && ElementClass::elementtype) != ElementClass::elementtype)
		return 0;

	return static_cast<ElementClass*>(element);
}

template <typename T> struct addIfCastable
{
	typedef void result_type;

	void operator()(ElementObject * element, std::vector<T*> & vct)
	{
		T * el = element_cast<T>(element);
		if(el)
			vct.push_back(el);
	}
};

template <typename ElementClass> std::vector<ElementClass*> findChildren(ElementObject * element)
{
	const std::vector<ElementObject * >	& children = element->children();
	std::vector<ElementClass*> transf;

	std::for_each(
			children.begin(),
			children.end(),
			boost::bind(
					addIfCastable<ElementClass>(),
					_1,
					boost::ref(transf))
			);

	return transf;
}



#endif // ELEMENTHELPER_HPP
