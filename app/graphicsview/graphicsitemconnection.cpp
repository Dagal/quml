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
#include "graphicsitemconnectionline.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QMenu>

GraphicsItemConnection::GraphicsItemConnection(QGraphicsItem * parent)
	: QGraphicsItem(parent)
{
	setFlag(QGraphicsItem::ItemIsFocusable, true);
}

GraphicsItemConnectionPoint * createNewPoint(const QPointF & position, GraphicsItemConnection * connection)
{
	GraphicsItemConnectionPoint * point = new GraphicsItemConnectionPoint(connection);
	point->setPos(position);
	point->addItemChangedListener(connection);
	if(connection->scene()) point->installSceneEventFilter(connection);

	return point;
}

GraphicsItemConnectionLine * createNewLine(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint, GraphicsItemConnection * connection)
{
	GraphicsItemConnectionLine * line = new GraphicsItemConnectionLine(startPoint, endPoint, connection);
	line->addItemChangedListener(connection);
	if(connection->scene()) line->installSceneEventFilter(connection);

	return line;
}

GraphicsItemConnectionPoint * GraphicsItemConnection::createPoint(int posInLine, const QPointF & posInScene)
{
	if(posInLine < 0 || posInLine > _points.size())
		posInLine = _points.size();

	GraphicsItemConnectionPoint * point = createNewPoint(posInScene, this);

	// first point, just insert
	if(_points.empty())
	{
		_points.push_back(point);
	}
	// front point
	else if(posInLine == 0)
	{
		_points.push_front(point);
		_lines.push_front(createNewLine(point, pointAt(0), this));
	}
	// end point
	else if(posInLine == _points.size())
	{
		_points.push_back(point);
		_lines.push_back(createNewLine(pointAt(posInLine-1), point, this));
	}
	// point in the middle
	else
	{
		_points.insert(posInLine, point);
		// get the old line
		GraphicsItemConnectionLine * oldLine = _lines[posInLine-1];

		// create the two new lines
		_lines[posInLine-1] = createNewLine(oldLine->startPoint(), point, this);
		_lines.insert(posInLine, createNewLine(point, oldLine->endPoint(), this));

		// destroy the old lines
		delete oldLine;
	}

	updateCompleteConnection();

	return point;
}

void GraphicsItemConnection::removePoint(int position)
{
	if(position < 0 || position >= _points.size())
		return;

	GraphicsItemConnectionPoint * point = pointAt(position);

	if(_points.size() == 1)
	{
		_points.removeAt(position);
	}
	else if(position == 0)
	{
		delete _lines.first();
		_lines.removeFirst();
		_points.removeAt(position);
	}
	else if(position == _points.size()-1)
	{
		delete _lines.last();
		_lines.removeLast();
		_points.removeAt(position);
	}
	else
	{
		// delete two lines (position-1 && position) and create one new
		GraphicsItemConnectionLine * lineA = _lines[position-1];
		GraphicsItemConnectionLine * lineB = _lines[position];

		// only remove one line, overwrite the second
		_points.removeAt(position);
		_lines[position] = createNewLine(lineA->startPoint(), lineB->endPoint(), this);
		_lines.removeAt(position-1);

		delete lineA;
		delete lineB;
	}

	delete point;

	updateCompleteConnection();
}

GraphicsItemConnectionPoint * GraphicsItemConnection::pointAt(int position) const
{
	if(position < 0 || position >= _points.size())
		return 0;
	else
		return _points.at(position);
}

int GraphicsItemConnection::pointPosition(GraphicsItemConnectionPoint * point) const
{
	return _points.indexOf(point);
}

void GraphicsItemConnection::updateCompleteConnection()
{
	//for(int i = 0; i < _points.size(); i++)
//		_points[i]->setFlag(QGraphicsItem::ItemIsMovable, !(i == 0 || i == _points.size()-1));

	for(int i = 0; i < _lines.size(); i++)
	{
		_lines[i]->setStartType(GraphicsItemConnectionLine::LineConnectionNormal);
		_lines[i]->setEndType(GraphicsItemConnectionLine::LineConnectionNormal);

		if(i == 0)
			_lines[i]->setStartType(GraphicsItemConnectionLine::LineConnectionStartArrow);
		if(i == _lines.size()-1)
			_lines[i]->setEndType(GraphicsItemConnectionLine::LineConnectionClosedEndArrow);
	}

	updateBoundingRect();
}

void GraphicsItemConnection::updateBoundingRect()
{
	// loop through everything and get the bounding box
	_boundingRect = QRectF();

	foreach(GraphicsItemConnectionPoint * point, _points)
	{
		QRectF itemRct = mapRectFromItem(point, point->boundingRect());
		_boundingRect |= itemRct;
	}

	foreach(GraphicsItemConnectionLine * line, _lines)
		_boundingRect |= mapRectFromItem(line, line->boundingRect());

}

void GraphicsItemConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	Q_UNUSED(painter);
}


bool GraphicsItemConnection::itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue)
{
	Q_UNUSED(item);
	Q_UNUSED(inValue);
	Q_UNUSED(outValue);

	if(change == QGraphicsItem::ItemPositionHasChanged)
		updateBoundingRect();

	return false;
}

QVariant GraphicsItemConnection::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if(change == QGraphicsItem::ItemChildAddedChange)
	{
		QGraphicsItem * newChild = value.value<QGraphicsItem*>();
		newChild->setFlag(QGraphicsItem::ItemIsFocusable, false);
		newChild->setFlag(QGraphicsItem::ItemStacksBehindParent, !hasFocus());

		GraphicsItemConnectionPoint * p = qgraphicsitem_cast<GraphicsItemConnectionPoint*>(newChild);
		if( p != 0)
		{
			p->setPointStatus( hasFocus() ? GraphicsItemConnectionPoint::PointVisible : GraphicsItemConnectionPoint::PointDisabled );

			if(hasFocus())
				p->setSelected(true);
		}
	}

	return QGraphicsItem::itemChange(change, value);
}

bool GraphicsItemConnection::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
	if(event->type() == QEvent::GraphicsSceneContextMenu)
	{
		GraphicsItemConnectionPoint * p = qgraphicsitem_cast<GraphicsItemConnectionPoint*>(watched);
		GraphicsItemConnectionLine * l = qgraphicsitem_cast<GraphicsItemConnectionLine*>(watched);

		if(p) return onPointContextMenu(p, static_cast<QGraphicsSceneContextMenuEvent*>(event));
		if(l) return onLineContextMenu(l, static_cast<QGraphicsSceneContextMenuEvent*>(event));

	}
	else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
	{
		GraphicsItemConnectionPoint * p = qgraphicsitem_cast<GraphicsItemConnectionPoint*>(watched);
		GraphicsItemConnectionLine * l = qgraphicsitem_cast<GraphicsItemConnectionLine*>(watched);

		if(p) return onPointDoubleClick(p, static_cast<QGraphicsSceneMouseEvent*>(event));
		if(l) return onLineDoubleClick(l, static_cast<QGraphicsSceneMouseEvent*>(event));
	}

	return QGraphicsItem::sceneEventFilter(watched, event);
}

bool GraphicsItemConnection::sceneEvent(QEvent * event)
{
	if(event->type() == QEvent::GraphicsSceneMousePress)
		return true;

	return QGraphicsItem::sceneEvent(event);
}

void GraphicsItemConnection::focusInEvent(QFocusEvent * event)
{
	Q_UNUSED(event);

	foreach(QGraphicsItem * item, childItems())
	{
		item->setSelected(false);
		item->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
	}

	foreach(GraphicsItemConnectionPoint * p, _points)
		p->setPointStatus(GraphicsItemConnectionPoint::PointVisible);
}

void GraphicsItemConnection::focusOutEvent(QFocusEvent * event)
{
	// no pop-up focus out events
	if(event->reason() == 4)
	{
		setFocus(Qt::OtherFocusReason);
		return;
	}


	foreach(QGraphicsItem * item, childItems())
	{
		item->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
		item->setSelected(false);
	}

	foreach(GraphicsItemConnectionPoint * p, _points)
		p->setPointStatus(GraphicsItemConnectionPoint::PointDisabled);
}


bool GraphicsItemConnection::onPointContextMenu(GraphicsItemConnectionPoint * point, QGraphicsSceneContextMenuEvent * event)
{
	QMenu m;
	QAction * delAction = m.addAction("Delete point");

	if(m.exec(event->screenPos(), 0) == delAction)
	{
		removePoint(pointPosition(point));
		return true;
	}

	return false;
}

bool GraphicsItemConnection::onLineContextMenu(GraphicsItemConnectionLine * line, QGraphicsSceneContextMenuEvent * event)
{
	Q_UNUSED(line);
	Q_UNUSED(event);

	return true;
}

bool GraphicsItemConnection::onPointDoubleClick(GraphicsItemConnectionPoint * point, QGraphicsSceneMouseEvent * event)
{
	Q_UNUSED(point);
	Q_UNUSED(event);

	return true;
}

bool GraphicsItemConnection::onLineDoubleClick(GraphicsItemConnectionLine * line, QGraphicsSceneMouseEvent * event)
{
	int posInLine = pointPosition(line->endPoint());
	QPointF posInScene = mapFromScene(event->scenePos());

	createPoint(posInLine, posInScene);

	return true;
}
