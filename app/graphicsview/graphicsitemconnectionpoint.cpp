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

#include "graphicsitemconnectionpoint.hpp"
#include "graphicsitemconnection.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

GraphicsItemConnectionPoint::GraphicsItemConnectionPoint(QGraphicsItem * parent)
	: QGraphicsRectItem(-3, -3, 7, 7, 0, 0),
	 _connection(0)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

	setBrush(QBrush(Qt::red, Qt::SolidPattern));

	setParentItem(parent);
}

void GraphicsItemConnectionPoint::addToConnection()
{
	GraphicsItemConnection * connection = qgraphicsitem_cast<GraphicsItemConnection*>(parentItem());
	if(!connection)
		return;

	_connection = connection;
	_connection->attachPoint(this, -1);
}

void GraphicsItemConnectionPoint::removeFromConnection()
{
	if(!_connection)
		return;

	_connection->detachPoint(this);
	_connection = 0;
}

QVariant GraphicsItemConnectionPoint::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if(change == ItemPositionHasChanged && _connection)
		_connection->updateConnection();
	else if(change == ItemParentChange)
		removeFromConnection();
	else if(change == ItemParentHasChanged)
		addToConnection();

	return QGraphicsItem::itemChange(change, value);
}
