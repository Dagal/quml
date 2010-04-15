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

#include <QList>
#include <QString>
#include <algorithm>

template <typename T> class ElementContainer
{
	struct elementfinder
	{
		elementfinder(const QString & elementName) : _elementName(elementName) {}

		bool operator()(const T * element) const
		{
			return element->name() == _elementName;
		}

	private:
		QString _elementName;
	};

	typedef QList<T*> vct;
	typedef typename QList<T*>::iterator vctIt;
	typedef typename QList<T*>::const_iterator vctConstIt;

public:
	bool addElement(T * newElement);
	bool removeElement(T * element);
	bool removeElement(const QString & elementName);
	bool hasElement(T * element) const;
	T * findElement(const QString & elementName) const;
	T * getElement(int position) const;
	unsigned int count() const;

	const QList<T*> & vector() const;

	typename QList<T*>::const_iterator findElementIterator(const QString & element) const;
	typename QList<T*>::iterator findElementIterator(const QString & element);

private:
	QList<T*> _elements;
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

template <typename T> bool ElementContainer<T>::removeElement(const QString & elementName)
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

template <typename T> T * ElementContainer<T>::findElement(const QString & elementName) const
{
	vctConstIt it = findElementIterator(elementName);
	if(it != _elements.end())
		return (*it);
	else
		return 0;
}
template <typename T> inline T * ElementContainer<T>::getElement(int position) const
{
	if(position >= _elements.size() || position < 0)
		return 0;
	else
		return _elements[position];
}
template <typename T> inline unsigned int ElementContainer<T>::count() const
{
	return _elements.size();
}
 template <typename T> inline const QList<T*> & ElementContainer<T>::vector() const
{
	return _elements;
}

template <typename T> inline typename QList<T*>::const_iterator ElementContainer<T>::findElementIterator(const QString & element) const
{
	return std::find_if(
			_elements.begin(),
			_elements.end(),
			elementfinder(element));
}
template <typename T> inline typename QList<T*>::iterator ElementContainer<T>::findElementIterator(const QString & element)
{
	return std::find_if(
			_elements.begin(),
			_elements.end(),
			elementfinder(element));
}

#endif // ELEMENTCONTAINER_HPP
