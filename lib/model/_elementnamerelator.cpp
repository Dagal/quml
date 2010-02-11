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

#include "_elementnamerelator.hpp"
#include <boost/bind.hpp>

void ElementNameRelator::Alterer::operator()(const std::string & newName)
{
	_functor(_element, newName);
}

void ElementNameRelator::addAlterer(ElementObject * element, const AltererFunctor & functor)
{
	if(!element)
		return;

	// remove an alterer if present
	removeAlterer(element);

	// add the alterer
	_alterers.push_back(Alterer(element, functor));
}

void ElementNameRelator::removeAlterer(ElementObject * object)
{
	std::remove_if(
			_alterers.begin(),
			_alterers.end(),
			boost::bind(&Alterer::_functor, _1) == object );
}

void ElementNameRelator::executeAlterers(const std::string & name)
{
	std::for_each(
			_alterers.begin(),
			_alterers.end(),
			boost::bind(&ElementNameRelator::Alterer::operator (), _1, name)
			);
}
