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

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <QHash>

class Parameter;

template <typename KEY, typename VALUE, typename KeyFunction> class Manager
{
public:
	void addItem(VALUE * item)
	{
		_map[_function(item)] = item;
	}
	bool hasItem(const KEY & key) const
	{
		return _map.contains(key);
	}
	bool hasItem(VALUE * item) const
	{
		return hasItem(_function(item));
	}
	VALUE * getItem(const KEY & key) const
	{
		if(hasItem)
			return _map[key];
		else
			return 0;
	}
	VALUE * takeItem(const KEY & key)
	{
		return _map.take(key);
	}
	void removeItem(const KEY & key)
	{
		_map.remove(key);
	}
	QList<VALUE*> allItems() const
	{
		return _map.values();
	}

private:
	QHash<KEY, VALUE *> _map;
	KeyFunction _function;
};

#define MANAGER(NAME) \
\
template <typename KEY, typename VALUE, typename KeyFunction> class NAME##Manager\
{\
public:\
	void add##NAME(VALUE * item)\
	{\
		_map##NAME[_function##NAME(item)] = item;\
	}\
	bool has##NAME(const KEY & key) const\
	{\
	 return _map##NAME.contains(key);\
	}\
	VALUE * get##NAME(const KEY & key) const\
	{\
		if(has##NAME(key))\
			return _map##NAME[key];\
		else\
			return 0;\
	}\
	bool has##NAME(VALUE * item) const\
	{\
	 return get##NAME(KeyFunction(item) != 0);\
	}\
	void remove##NAME(const KEY & key)\
	{\
		_map##NAME.remove(key);\
	}\
	VALUE * take##NAME(const KEY & key)\
	{\
		return _map##NAME.take(key);\
	}\
	const QHash<KEY, VALUE*> & get##NAME##Map() const\
	{\
		return _map##NAME;\
	}\
\
public:\
	QHash<KEY, VALUE*> _map##NAME;\
	KeyFunction _function##NAME;\
}\

#endif // MANAGER_HPP
