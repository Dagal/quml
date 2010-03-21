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
#include "graphicsitemconnectionline.hpp"
#include <QPainter>
#include <QBrush>

const float rectWidth = 4;

GraphicsItemConnectionPoint::GraphicsItemConnectionPoint(QGraphicsItem * parent)
	: GraphicsExt<QGraphicsRectItem>(0),
	_pointStatus(PointDisabled)
{
	setRect(-rectWidth, -rectWidth, rectWidth*2+1,rectWidth*2+1);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	//setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

	setBrush(QBrush(Qt::NoBrush));
	setSelectedBrush(QBrush(Qt::red, Qt::SolidPattern));
	setSelectedPen(QPen(Qt::NoPen));
	setParentItem(parent);
}

void GraphicsItemConnectionPoint::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	switch(pointStatus())
	{
	case PointDisabled:
		return;
		break;

	case PointVisible:
		painter->setBrush(brush());
		painter->setPen(pen());
		break;

	case PointSelected:
		painter->setBrush(selectedBrush());
		painter->setPen(selectedPen());
		break;
	}

	painter->drawRect(rect());
}

void GraphicsItemConnectionPoint::setPointStatus(PointStatus  pointStatus)
{
	if(pointStatus == _pointStatus)
		return;

	_pointStatus = pointStatus;
	setSelected(pointStatus == PointSelected);

	update();
}

QVariant GraphicsItemConnectionPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
{
	if(change == QGraphicsItem::ItemSelectedHasChanged)
	{
		if(value.toBool() && pointStatus() != PointSelected) setPointStatus(PointSelected);
		else if(!value.toBool() && pointStatus() == PointSelected) setPointStatus(PointVisible);
	}

	return GraphicsExt<QGraphicsRectItem>::itemChange(change, value);
}
