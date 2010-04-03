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

#include "graphicsitemrelation.hpp"
#include "connection/graphicsitemconnectionpoint.hpp"
#include "connection/graphicsitemconnectionline.hpp"
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QDebug>

using namespace connection;

GraphicsItemRelation::GraphicsItemRelation(QGraphicsItem * parent)
	: GraphicsItemConnection(parent)
{
}

void GraphicsItemRelation::onLineCreated(GraphicsItemConnectionLine * newLine)
{
	if(scene()) newLine->installSceneEventFilter(this);
}

void GraphicsItemRelation::onPointAdded(GraphicsItemConnectionPoint * newPoint)
{
	if(scene()) newPoint->installSceneEventFilter(this);
}

void GraphicsItemRelation::onPointRemoved(GraphicsItemConnectionPoint * oldPoint)
{
	if(scene()) oldPoint->removeSceneEventFilter(this);
}

bool GraphicsItemRelation::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
	if(event->type() == QEvent::GraphicsSceneContextMenu)
	{
		GraphicsItemConnectionPoint * p = qgraphicsitem_cast<GraphicsItemConnectionPoint*>(watched);
		if(p) return onPointContextMenu(p, static_cast<QGraphicsSceneContextMenuEvent*>(event));

		GraphicsItemConnectionLine * l = qgraphicsitem_cast<GraphicsItemConnectionLine*>(watched);
		if(l) return onLineContextMenu(l, static_cast<QGraphicsSceneContextMenuEvent*>(event));
	}
	else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick)
	{
		GraphicsItemConnectionLine * l = qgraphicsitem_cast<GraphicsItemConnectionLine*>(watched);
		if(l) return onLineDoubleClick(l, static_cast<QGraphicsSceneMouseEvent*>(event));
	}

	return GraphicsItemConnection::sceneEventFilter(watched, event);
}

bool GraphicsItemRelation::onPointContextMenu(GraphicsItemConnectionPoint * point, QGraphicsSceneContextMenuEvent * event)
{
	QMenu m;
	QAction * delAction = m.addAction("Delete point");

	if(m.exec(event->screenPos(), 0) == delAction)
	{
		delete detachPoint(pointPosition(point));
		return true;
	}

	return false;
}

bool GraphicsItemRelation::onLineContextMenu(GraphicsItemConnectionLine * line, QGraphicsSceneContextMenuEvent * event)
{
	Q_UNUSED(line);
	Q_UNUSED(event);

	return true;
}

bool GraphicsItemRelation::onLineDoubleClick(GraphicsItemConnectionLine * line, QGraphicsSceneMouseEvent * event)
{
	int posInLine = pointPosition(line->endPoint());
	QPointF posInScene = mapFromScene(event->scenePos());

	GraphicsItemConnectionPoint * p = new GraphicsItemConnectionPoint(this);
	p->setPos(posInScene);

	attachPoint(p, posInLine);

	return true;
}

