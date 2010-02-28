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

#ifndef ELEMENTCONTAINER_HPP
#define ELEMENTCONTAINER_HPP

#include <vector>
#include <string>
#include <algorithm>

template <typename T> class ElementContainer
{
	struct elementfinder
	{
		elementfinder(const std::string & elementName) : _elementName(elementName) {}

		bool operator()(const T * element) const
		{
			return element->name() == _elementName;
		}

	private:
		std::string _elementName;
	};

	typedef typename std::vector<T*> vct;
	typedef typename std::vector<T*>::iterator vctIt;
	typedef typename std::vector<T*>::const_iterator vctConstIt;

public:
	bool addElement(T * newElement);
	bool removeElement(T * element);
	bool removeElement(const std::string & elementName);
	bool hasElement(T * element) const;
	T * findElement(const std::string & elementName) const;
	T * getElement(unsigned int position) const;
	unsigned int count() const;

	const std::vector<T*> & vector() const;

	typename std::vector<T*>::const_iterator findElementIterator(const std::string & element) const;
	typename std::vector<T*>::iterator findElementIterator(const std::string & element);

private:
	std::vector<T*> _elements;
};

template <typename T> bool ElementContainer<T>::addElement(T * newElement)
{
	if(!newElement || hasElement(newElement))	return false;
	_elements.push_back(newElement);
	return true;
}

template <typename T> bool ElementContainer<T>::removeElement(T * element)
{
	if(!element) return false;

	vctIt it = std::remove(
			_elements.begin(),
			_elements.end(),
			element);

	if(it == _elements.end())
		return false;

	_elements.erase(it);
	return true;
}

template <typename T> bool ElementContainer<T>::removeElement(const std::string & elementName)
{
	vctIt it = findElement(elementName);
	if(it == _elements.end())
		return false;

	_elements.erase(it);
	return true;
}

template <typename T> inline bool ElementContainer<T>::hasElement(T * element) const
{
	return (std::find(_elements.begin(), _elements.end(), element) != _elements.end());
}

template <typename T> T * ElementContainer<T>::findElement(const std::string & elementName) const
{
	vctConstIt it = findElementIterator(elementName);
	if(it != _elements.end())
		return (*it);
	else
		return 0;
}
template <typename T> inline T * ElementContainer<T>::getElement(unsigned int position) const
{
	if(position >= _elements.size())
		return 0;
	else
		return _elements[position];
}
template <typename T> inline unsigned int ElementContainer<T>::count() const
{
	return _elements.size();
}
 template <typename T> inline const std::vector<T*> & ElementContainer<T>::vector() const
{
	return _elements;
}

template <typename T> inline typename std::vector<T*>::const_iterator ElementContainer<T>::findElementIterator(const std::string & element) const
{
	return std::find_if(
			_elements.begin(),
			_elements.end(),
			elementfinder(element));
}
template <typename T> inline typename std::vector<T*>::iterator ElementContainer<T>::findElementIterator(const std::string & element)
{
	return std::find_if(
			_elements.begin(),
			_elements.end(),
			elementfinder(element));
}

#endif // ELEMENTCONTAINER_HPP
