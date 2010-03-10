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

#include "graphicsitemconnection.hpp"
#include "graphicsitemconnectionpoint.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

GraphicsItemConnection::GraphicsItemConnection(QGraphicsItem * parent)
	: QGraphicsPathItem(parent),
	_shiftDown(false)
{
}


void GraphicsItemConnection::updateConnection()
{
	if(_points.size() == 0)
		setPath(QPainterPath());

	QPainterPath p(_points[0]->pos());

	for(int i = 0; i < _points.size(); i++)
		p.lineTo(_points[i]->pos());

	setPath(p);
}

void GraphicsItemConnection::attachPoint(GraphicsItemConnectionPoint * point, int position)
{
	if(!point)
		return;

	if(point->parentItem() != this)
	{
		point->setParentItem(this);
		return;
	}

	if(_points.contains(point))
		return;

	if(position < 0) position = _points.size();
	_points.insert(position, point);
	if(scene()) point->installSceneEventFilter(this);
	updateConnection();
}

void GraphicsItemConnection::detachPoint(GraphicsItemConnectionPoint * point)
{
		if(!point)
		return;

	_points.removeAll(point);
	if(scene()) point->removeSceneEventFilter(this);
	updateConnection();
}

QVariant GraphicsItemConnection::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if(change == QGraphicsItem::ItemSceneChange)
		removeAllSceneEventFilters();
	else if(change == QGraphicsItem::ItemSceneHasChanged)
		installAllSceneEventFilters();

	return value;
}

void GraphicsItemConnection::removeAllSceneEventFilters()
{
	if(scene())
		foreach(GraphicsItemConnectionPoint * point, _points)
			point->removeSceneEventFilter(this);
}

void GraphicsItemConnection::installAllSceneEventFilters()
{
	if(scene())
		foreach(GraphicsItemConnectionPoint * point, _points)
			point->installSceneEventFilter(this);
}

GraphicsItemConnectionPoint * GraphicsItemConnection::pointAt(int position) const
{
	if(position < 0)
		return 0;
	if(position >= _points.size())
		return 0;

	return _points.at(position);
}

int GraphicsItemConnection::findPointPosition(GraphicsItemConnectionPoint * point) const
{
	return _points.indexOf(point);
}

bool GraphicsItemConnection::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
	if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
	{
		QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
		if(keyEvent->key() == Qt::Key_Shift)
			_shiftDown =event->type() == QEvent::KeyPress;
	}
	else if(event->type() == QEvent::GraphicsSceneMouseMove)
	{
		return filterMouseMovement(
				qgraphicsitem_cast<GraphicsItemConnectionPoint*>(watched),
				static_cast<QGraphicsSceneMouseEvent*>(event));
	}

	return false;
}

bool GraphicsItemConnection::filterMouseMovement(GraphicsItemConnectionPoint * point, QGraphicsSceneMouseEvent * event)
{
	const float distanceToLock = 10.0f;

	if(!_shiftDown || !point)
		return false;

	QPointF position = point->mapToParent(event->pos());

	// get the two other points
	int pointPos = findPointPosition(point);
	GraphicsItemConnectionPoint * prev = pointAt(pointPos-1);
	GraphicsItemConnectionPoint * next = pointAt(pointPos+1);

	if(prev == 0 && next == 0)
		return false;

	QRectF rct;
	if(prev == 0)
		rct = QRectF(position, next->pos());
	else if(next == 0)
		rct = QRectF(prev->pos(), position);
	else
		rct = QRectF(prev->pos(), next->pos());

	QPointF d1 = rct.topRight()-position;
	QPointF d2 = rct.bottomLeft()-position;

	qreal p1 = d1.x()*d1.x() + d1.y()*d1.y();
	qreal p2 = d2.x()*d2.x() + d2.y()*d2.y();

	QPointF realPoint (p1 < p2 ? d1 : d2);

	if(qAbs(realPoint.x()) < distanceToLock) position.rx() += realPoint.x();
	if(qAbs(realPoint.y()) < distanceToLock) position.ry() += realPoint.y();

	point->setPos(position);
	return true;
}

