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

#ifndef IEXTENDEDGRAPHICSITEM_HPP
#define IEXTENDEDGRAPHICSITEM_HPP

#include <QGraphicsItem>

class ItemChangedListener;

struct GraphicsExtPrivate
{
	~GraphicsExtPrivate();

	void addItemChangedListener(ItemChangedListener * listener);
	void removeItemChangedListener(ItemChangedListener * listener);
	const QList<ItemChangedListener*> & listeners() const;
	bool sendItemChanged(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & value, QVariant & outValue);

private:
	QList<ItemChangedListener*> _listeners;
};

class ItemChangedListener
{
	friend class GraphicsExtPrivate;

public:
	~ItemChangedListener();

protected:
	virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

private:
	QList<GraphicsExtPrivate*> _listenObjects;
};

template <class T> class GraphicsExt : public T, public ItemChangedListener
{
public:
	GraphicsExt(QGraphicsItem * parent = 0)
		: T(parent), _dd(new GraphicsExtPrivate)
	{
	}

	void addItemChangedListener(ItemChangedListener * listener)
	{
		_dd->addItemChangedListener(listener);
	}

	void removeItemChangedListener(ItemChangedListener * listener)
	{
		_dd->removeItemChangedListener(listener);
	}

protected:
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
	{
		QVariant outValue = value;
		if(_dd->sendItemChanged(this, change, value,outValue))
			return outValue;

		return T::itemChange(change, value);
	}

private:
	QSharedPointer<GraphicsExtPrivate> _dd;
};



#endif // IEXTENDEDGRAPHICSITEM_HPP
