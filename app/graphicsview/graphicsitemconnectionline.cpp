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

#include "graphicsitemconnectionline.hpp"
#include "graphicsitemconnectionpoint.hpp"
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <cmath>

const float dxArrow = 12;
const float dyArrow = 5;
const float dxDiamond = 12;
const float dyDiamond = 5;

QMap<GraphicsItemConnectionLine::ConnectionType, GraphicsItemConnectionLine::MarkerInformation> GraphicsItemConnectionLine::_Markers = QMap<GraphicsItemConnectionLine::ConnectionType, GraphicsItemConnectionLine::MarkerInformation>();

void GraphicsItemConnectionLine::InitialiseMarkers()
{
	if(_Markers.size() != 0)
		return;

	QPainterPath pAggregation;
	pAggregation.lineTo(dxDiamond, dyDiamond);
	pAggregation.lineTo(dxDiamond*2, 0);
	pAggregation.lineTo(dxDiamond, -dyDiamond);
	pAggregation.lineTo(0,0);
	pAggregation.moveTo(dxDiamond*2, 0);

	QPainterPath pStartArrow;
	pStartArrow.lineTo(dxArrow, dyArrow);
	pStartArrow.moveTo(0,0);
	pStartArrow.lineTo(dxArrow, -dyArrow);
	pStartArrow.moveTo(0,0);

	QPainterPath pEndArrow;
	pEndArrow.lineTo(-dxArrow, dyArrow);
	pEndArrow.moveTo(0,0);
	pEndArrow.lineTo(-dxArrow, -dyArrow);
	pEndArrow.moveTo(0,0);

	QPainterPath pClosedStartArrow;
	pClosedStartArrow.lineTo(dxArrow, dyArrow);
	pClosedStartArrow.lineTo(dxArrow, -dyArrow);
	pClosedStartArrow.lineTo(0,0);
	pClosedStartArrow.moveTo(dxArrow, 0);

	QPainterPath pClosedEndArrow;
	pClosedEndArrow.lineTo(-dxArrow, dyArrow);
	pClosedEndArrow.lineTo(-dxArrow, -dyArrow);
	pClosedEndArrow.lineTo(0,0);
	pClosedEndArrow.moveTo(-dxArrow, 0);

	QPainterPath pNormal;

	_Markers[LineConnectionNormal] = MarkerInformation(pNormal, false);
	_Markers[LineConnectionStartArrow] = MarkerInformation(pStartArrow, false);
	_Markers[LineConnectionEndArrow] = MarkerInformation(pEndArrow, false);
	_Markers[LineConnectionClosedStartArrow] = MarkerInformation(pClosedStartArrow, false);
	_Markers[LineConnectionClosedEndArrow] = MarkerInformation(pClosedEndArrow, false);
	_Markers[LineConnectionAggregation] = MarkerInformation(pAggregation, false);
	_Markers[LineConnectionComposition] = MarkerInformation(pAggregation, true);
}

GraphicsItemConnectionLine::GraphicsItemConnectionLine(GraphicsItemConnectionPoint * start, GraphicsItemConnectionPoint * end, QGraphicsItem * parent)
	: GraphicsExt<QGraphicsPathItem>(parent),
	_startPoint(start),
	_endPoint(end)
{
	_startPoint->addItemChangedListener(this);
	_endPoint->addItemChangedListener(this);

	InitialiseMarkers();
	initialiseDefaults();

	updatePosition();
}
GraphicsItemConnectionLine::~GraphicsItemConnectionLine()
{
	_startPoint->removeItemChangedListener(this);
	_endPoint->removeItemChangedListener(this);
}

void GraphicsItemConnectionLine::initialiseDefaults()
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	// make sure this items have a z-value lower that the connectors
	qreal z = qMin(_startPoint->zValue(), _endPoint->zValue());
	setZValue(z-1);

	_startType = LineConnectionAggregation;
	_endType = LineConnectionAggregation;

	setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 1));
	setSelectedPen(QPen(QBrush(Qt::blue, Qt::SolidPattern), 1));
	setBrush(QBrush(Qt::red, Qt::SolidPattern));
	setEmptyBrush(QBrush(Qt::white, Qt::SolidPattern));
}

void GraphicsItemConnectionLine::updatePosition()
{
	setTransform(QTransform());

	QPointF sp = mapFromItem(startPoint(), 0, 0);
	QPointF ep = mapFromItem(endPoint(), 0, 0);
	qreal angle = atan2(ep.y() - sp.y(), ep.x() - sp.x());

	// transform this "line", only rotating and translating
	QTransform t;
	t.translate(sp.x(), sp.y()).rotateRadians(angle);
	setTransform(t);

	float distance = mapFromItem(endPoint(), 0, 0).x();

	QPainterPath p;

	// create the right path
	_startMarker = (_Markers.contains(startType()) ? _Markers[startType()] : MarkerInformation());
	_endMarker = (_Markers.contains(endType()) ? _Markers[endType()] : MarkerInformation());
	_middleStart = _startMarker._path.currentPosition().x();

	_middleEnd = distance + _endMarker._path.currentPosition().x();

	_endMarker._path.translate(distance, 0);

	p += _startMarker._path + _endMarker._path;
	p.moveTo(_middleStart, 0);
	p.lineTo(_middleEnd, 0);

	setPath(p);
}

void GraphicsItemConnectionLine::setStartType(ConnectionType type)
{
	if(_startType == type) return;

	_startType = type;
	updatePosition();
}

void GraphicsItemConnectionLine::setEndType(ConnectionType type)
{
	if(_endType == type) return;

	_endType = type;
	updatePosition();
}

void GraphicsItemConnectionLine::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(isSelected() ? selectedPen() : pen());

	painter->setBrush( (_startMarker._fill ? brush() : emptyBrush()) );
	painter->drawPath( _startMarker._path);

	painter->setBrush( (_endMarker._fill ? brush() : emptyBrush()) );
	painter->drawPath( _endMarker._path);

	painter->drawLine(_middleStart, 0, _middleEnd, 0);
}

bool GraphicsItemConnectionLine::itemChangedFilter(QGraphicsItem * /*item*/, QGraphicsItem::GraphicsItemChange change, const QVariant & /*inValue*/, QVariant & /*outValue*/)
{
	if(change == QGraphicsItem::ItemPositionHasChanged)
		updatePosition();

	return false;
}
