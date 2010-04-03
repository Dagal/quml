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

/*!
  \brief This namespace contains the classes for extending the QGraphicsView Framework.

  This namespace contains the classes for extending the QGraphicsView Framework. The ItemChangedListener and the GraphicsExt class are used to forward
  the itemChange to different listeners.
*/
namespace graphicsUtil
{
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


	/*!
	  This ItemChangedListeners is the base type for listeners used in GraphicsExt.
	*/
	class ItemChangedListener
	{
		friend class GraphicsExtPrivate;

	public:
		~ItemChangedListener();

	protected:
		/*!
		  This function notifies the user of the fact that \c item has changed. The value of \c change and \c inValue are those used in the itemChange function.
		  Reimplement this method to receive notifications and/or to intercept them.

		  By returning true, the event is intercepted and the value stored in \c outValue is used as return value for itemChange. By returning false, the
		  notification is propagated to the other listeners.
		*/
		virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

	private:
		QList<GraphicsExtPrivate*> _listenObjects;
	};

	/*!
	  The GraphicsExt template class is used to extend the QGraphicsView Framework. The main goal is to forward itemChange messages towards different
	  listeners. The listeners can intercept and change the message. As soon as one listener intercepts the message, the message is no longer distributed
	  to the other listeners.

	  GraphicsExt inherits from its template parameter \c T.

	  \sa GraphicsExt::itemChange
	*/
	template <class T> class GraphicsExt : public T, public ItemChangedListener
	{
	public:
		/*!
		  Creates a GraphicsExt and passes the value of parent on to the template parameter \c T.
		*/
		GraphicsExt(QGraphicsItem * parent = 0)
			: T(parent), _dd(new GraphicsExtPrivate)
		{
		}

		/*!
		  Use this function to add new listeners to this element. Each of these listeners will receive the itemChange functions and will be able to intercept them.
		  The first added listener will be called first.
		*/
		void addItemChangedListener(ItemChangedListener * listener)
		{
			_dd->addItemChangedListener(listener);
		}

		/*!
		  Use this function to remove listeners from this element. When a listener is destroyed, he will automatically be remove from the listeners.
		*/
		void removeItemChangedListener(ItemChangedListener * listener)
		{
			_dd->removeItemChangedListener(listener);
		}

	protected:
		/*!
		  This is the reimplement method from the QGraphicsView Framework. If you would reimplement this method in a subclass, make sure to call this base
		  method. Otherwise the listeners will not receive any information.
		*/
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
}


#endif // IEXTENDEDGRAPHICSITEM_HPP
