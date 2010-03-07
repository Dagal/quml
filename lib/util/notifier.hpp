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

#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <vector>
#include <boost/bind.hpp>
#include <algorithm>

template <typename T> class Listener;

template <typename T> class Notifier
{
	typedef std::vector<Listener<T> *> listenerVct;
	typedef typename std::vector<Listener<T> *>::iterator listenerIterator;

public:
	void addEventListener(Listener<T> * listener);
	void removeEventListener(Listener<T> * listener);
	bool hasEventListener(Listener<T> * listener) const;

	// message functions
	void sendMessage(const T & message) const;

private:
	listenerIterator findListener(Listener<T> * listener);

private:
	listenerVct _listeners;
};





template <typename T> inline void Notifier<T>::addEventListener(Listener<T> * listener)
{
	if(listener == 0 || hasEventListener(listener))
		return;

	_listeners.push_back(listener);
}

template <typename T> void Notifier<T>::removeEventListener(Listener<T> * listener)
{
	listenerIterator i = findListener(listener);
	if(i != _listeners.end())
		_listeners.erase(i);
}

template <typename T> inline  bool Notifier<T>::hasEventListener(Listener<T> * listener) const
{
	return findListener(listener) != _listeners.end();
}

template <typename T> void Notifier<T>::sendMessage(const T & message) const
{
	std::for_each(
			_listeners.begin(),
			_listeners.end(),
			boost::bind(&Listener<T>::messageReceived, _1, boost::cref(message))
			);

}

template <typename T> inline typename Notifier<T>::listenerIterator Notifier<T>::findListener(Listener<T> * listener)
{
	return std::find(
			_listeners.begin(),
			_listeners.end(),
			listener);
}


#endif // NOTIFIER_HPP
