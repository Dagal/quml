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

namespace connection
{
	QMap<GraphicsItemConnectionLine::ConnectionType, GraphicsItemConnectionLine::MarkerInformation> GraphicsItemConnectionLine::_Markers = QMap<GraphicsItemConnectionLine::ConnectionType, GraphicsItemConnectionLine::MarkerInformation>();

	/*!
	  This method adds the basic marker types to the static MarkerMap.
	  \todo Put this function in the PIMPL-pattern
	*/
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
		pEndArrow.moveTo(0, dyArrow);
		pEndArrow.lineTo(dxArrow, 0);
		pEndArrow.lineTo(0, -dyArrow);
		pEndArrow.moveTo(dxArrow, 0);


		QPainterPath pClosedStartArrow;
		pClosedStartArrow.lineTo(dxArrow, dyArrow);
		pClosedStartArrow.lineTo(dxArrow, -dyArrow);
		pClosedStartArrow.lineTo(0,0);
		pClosedStartArrow.moveTo(dxArrow, 0);

		QPainterPath pClosedEndArrow;
		pClosedEndArrow.moveTo(0, dyArrow);
		pClosedEndArrow.lineTo(dxArrow, 0);
		pClosedEndArrow.lineTo(0, -dyArrow);
		pClosedEndArrow.lineTo(0, dyArrow);
		pClosedEndArrow.moveTo(dxArrow, 0);

		QPainterPath pNormal;

		_Markers[LineConnectionNormal] = MarkerInformation(pNormal, false);
		_Markers[LineConnectionStartArrow] = MarkerInformation(pStartArrow, false);
		_Markers[LineConnectionEndArrow] = MarkerInformation(pEndArrow, false);
		_Markers[LineConnectionClosedStartArrow] = MarkerInformation(pClosedStartArrow, false);
		_Markers[LineConnectionClosedEndArrow] = MarkerInformation(pClosedEndArrow, false);
		_Markers[LineConnectionAggregation] = MarkerInformation(pAggregation, false);
		_Markers[LineConnectionComposition] = MarkerInformation(pAggregation, true);
	}

	/*!
	  This constructor creates a default Line (based on GraphicsExt<QGraphicsPathItem>) connection start to end. An item changed listener is installed
	  on both points so that the line receives notification whenever one of the points is moved. The default line type is line with no connectors.
	*/
	GraphicsItemConnectionLine::GraphicsItemConnectionLine(GraphicsItemConnectionPoint * start, GraphicsItemConnectionPoint * end, QGraphicsItem * parent)
		: graphicsUtil::GraphicsExt<QGraphicsPathItem>(parent),
		_startPoint(start),
		_endPoint(end)
	{
		_startPoint->addItemChangedListener(this);
		_endPoint->addItemChangedListener(this);

		InitialiseMarkers();
		initialiseDefaults();

		updatePosition();
	}

	/*!
	  The destructor destroys the line and removes the point listeners.
	*/
	GraphicsItemConnectionLine::~GraphicsItemConnectionLine()
	{
		_startPoint->removeItemChangedListener(this);
		_endPoint->removeItemChangedListener(this);
	}

	/*!
	  \todo Move to PIMPL-implementation
	  This method just sets the basic settings for a line
	*/
	void GraphicsItemConnectionLine::initialiseDefaults()
	{
		setFlag(QGraphicsItem::ItemIsSelectable, true);

		// make sure this items have a z-value lower that the connectors
		qreal z = qMin(_startPoint->zValue(), _endPoint->zValue());
		setZValue(z-1);

		_startType = LineConnectionNormal;
		_endType = LineConnectionNormal;

		setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 1));
		setSelectedPen(QPen(QBrush(Qt::blue, Qt::SolidPattern), 1));
		setBrush(QBrush(Qt::red, Qt::SolidPattern));
		setEmptyBrush(QBrush(Qt::white, Qt::SolidPattern));
	}

	/*!
	  This helper function is called whenever one of the points is moved or a connection is changed and updates the line transformation and the connectors
	  \todo Move to PIMPL-implementation
	*/
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

		_middleEnd = distance - _endMarker._path.currentPosition().x();

		_endMarker._path.translate(_middleEnd, 0);

		p += _startMarker._path + _endMarker._path;
		p.moveTo(_middleStart, 0);
		p.lineTo(_middleEnd, 0);

		setPath(p);
	}

	/*!
	  Updates the start connection type and redraws the line
	*/
	void GraphicsItemConnectionLine::setStartType(ConnectionType type)
	{
		if(_startType == type) return;

		_startType = type;
		updatePosition();
	}

	/*!
	  Updates the end connection type and redraws the line
	*/
	void GraphicsItemConnectionLine::setEndType(ConnectionType type)
	{
		if(_endType == type) return;

		_endType = type;
		updatePosition();
	}

	/*!
	  This function draws the line. If the line is selected, the GraphicsItemConnectionLine::selectedPen is used, otherwise the normal
	  GraphicsItemConnectionLine::pen. If one of markers should be filled, the GraphicsItemConnectionLine::brush is used, otherwise
	  the GraphicsItemConnectionLine::emptyBrush is used.
	*/
	void GraphicsItemConnectionLine::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		Q_UNUSED(option);
		Q_UNUSED(widget);

		painter->setPen(isSelected() ? selectedPen() : pen());

		painter->setBrush( (_startMarker._fill ? brush() : emptyBrush()) );
		painter->drawPath( _startMarker._path);

		painter->setBrush( (_endMarker._fill ? brush() : emptyBrush()) );
		painter->drawPath( _endMarker._path);

		painter->drawLine(_middleStart, 0, _middleEnd, 0);
	}

	/*!
	  This method is reimplemented to listen to the position changes of the points. If a position is changed, the line is updated
	*/
	bool GraphicsItemConnectionLine::itemChangedFilter(QGraphicsItem * /*item*/, QGraphicsItem::GraphicsItemChange change, const QVariant & /*inValue*/, QVariant & /*outValue*/)
	{
		if(change == QGraphicsItem::ItemPositionHasChanged)
			updatePosition();

		return false;
	}
}
