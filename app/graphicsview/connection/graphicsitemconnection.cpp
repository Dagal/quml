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

GraphicsItemConnectionLine * DefaultLineCreator::operator()(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint)
{
	return new GraphicsItemConnectionLine(startPoint, endPoint);
}

GraphicsItemConnection::GraphicsItemConnection(QGraphicsItem * parent)
	: QGraphicsItem(parent)
	, _lineCreator(new DefaultLineCreator)
{
	setFlag(QGraphicsItem::ItemIsFocusable, true);
}

GraphicsItemConnection::~GraphicsItemConnection()
{
	delete _lineCreator;
}

GraphicsItemConnectionLine * GraphicsItemConnection::createAndAttachLine(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint, int position)
{
	if(!lineCreator())
		return 0;

	GraphicsItemConnectionLine * line = (*lineCreator())(startPoint, endPoint);
	line->setParentItem(this);

	_lines[position] = line;

	onLineCreated(line);

	return line;
}

void GraphicsItemConnection::attachPoint(GraphicsItemConnectionPoint * point, int position)
{
	if(point == 0)
		return;

	if(point->parentItem() != this)
		point->setParentItem(this);

	if(position < 0 || position > _points.size())
		position = _points.size();

	point->addItemChangedListener(this);

	// first point, just insert
	if(_points.empty())
	{
		_points.push_back(point);
	}
	// front point
	else if(position == 0)
	{
		_points.push_front(point);
		_lines.push_front(0);
		createAndAttachLine(point, pointAt(0), 0);
	}
	// end point
	else if(position == _points.size())
	{
		_points.push_back(point);
		_lines.push_back(0);
		createAndAttachLine(pointAt(position-1), point, _lines.size()-1);
	}
	// point in the middle
	else
	{
		_points.insert(position, point);
		// get the old line
		GraphicsItemConnectionLine * oldLine = _lines[position-1];

		// create the two new lines
		_lines.insert(position , 0);
		createAndAttachLine(oldLine->startPoint(), point, position-1);
		createAndAttachLine(point, oldLine->endPoint(), position);

		// destroy the old lines
		onLineDestroyed(oldLine);
		delete oldLine;
	}

	onPointAdded(point);

	updateCompleteConnection();
}

 GraphicsItemConnectionPoint * GraphicsItemConnection::detachPoint(int position)
{
	if(position < 0 || position >= _points.size())
		return 0;

	GraphicsItemConnectionPoint * point = pointAt(position);

	if(_points.size() == 1)
	{
		_points.removeAt(position);
	}
	else if(position == 0)
	{
		onLineDestroyed(_lines.first());
		delete _lines.first();
		_lines.removeFirst();
		_points.removeAt(position);
	}
	else if(position == _points.size()-1)
	{
		onLineDestroyed(_lines.last());
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
		createAndAttachLine(lineA->startPoint(), lineB->endPoint(), position);
		_lines.removeAt(position-1);

		onLineDestroyed(lineA);
		onLineDestroyed(lineB);
		delete lineA;
		delete lineB;
	}

	updateCompleteConnection();

	onPointRemoved(point);
	return point;
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
	onUpdateConnection();
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
	else if(change == QGraphicsItem::ItemChildRemovedChange)
	{
		QGraphicsItem * oldChild = value.value<QGraphicsItem*>();

		// detaching a connected point?
		GraphicsItemConnectionPoint * p = qgraphicsitem_cast<GraphicsItemConnectionPoint*>(oldChild);
		if(p)
		{
			int pos = pointPosition(p);
			if(pos != -1)
				detachPoint(pos);
		}

		// detaching a connected line?
		GraphicsItemConnectionLine * l = qgraphicsitem_cast<GraphicsItemConnectionLine*>(oldChild);
		if(l)
		{
			int pos = _lines.indexOf(l);
			if(pos != -1)
			{
				qDebug() << "GraphicsConnectionLine: Warning, don't remove a line from a connection!";
				createAndAttachLine(pointAt(pos), pointAt(pos+1), pos);
				updateCompleteConnection();
			}
		}
	}

	return QGraphicsItem::itemChange(change, value);
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

void GraphicsItemConnection::onUpdateConnection() {}
void GraphicsItemConnection::onPointAdded(GraphicsItemConnectionPoint * ) {}
void GraphicsItemConnection::onPointRemoved(GraphicsItemConnectionPoint * ) {}
void GraphicsItemConnection::onLineCreated(GraphicsItemConnectionLine * ) {}
void GraphicsItemConnection::onLineDestroyed(GraphicsItemConnectionLine * ) {}
