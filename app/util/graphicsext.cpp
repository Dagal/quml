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


#include "graphicsext.hpp"

namespace graphicsUtil
{
	GraphicsExtPrivate::~GraphicsExtPrivate()
	{
		while(!_listeners.empty())
			removeItemChangedListener(_listeners.first());
	}

	void GraphicsExtPrivate::addItemChangedListener(ItemChangedListener * listener)
	{
		if(listener == 0)
			return;

		if(_listeners.contains(listener))
			return;

		_listeners.push_back(listener);
		listener->_listenObjects.push_back(this);
	}

	void GraphicsExtPrivate::removeItemChangedListener(ItemChangedListener * listener)
	{
		_listeners.removeAll(listener);
		listener->_listenObjects.removeAll(this);
	}

	bool GraphicsExtPrivate::sendItemChanged(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & value, QVariant & outValue)
	{
		foreach(ItemChangedListener * listener, _listeners)
			if(listener->itemChangedFilter(item, change, value, outValue))
				return true;

		return false;
	}

	ItemChangedListener::~ItemChangedListener()
	{
		while(!_listenObjects.empty())
			_listenObjects.first()->removeItemChangedListener(this);
	}

	bool ItemChangedListener::itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue)
	{
		Q_UNUSED(item);
		Q_UNUSED(change);
		Q_UNUSED(inValue);
		Q_UNUSED(outValue);

		return false;
	}
}
