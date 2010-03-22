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

#ifndef GRAPHICSITEMCONNECTIONLINE_HPP
#define GRAPHICSITEMCONNECTIONLINE_HPP

#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include "macro.hpp"
#include "../util/iextendedgraphicsitem.hpp"

class GraphicsItemConnectionPoint;

class GraphicsItemConnectionLine : public GraphicsExt<QGraphicsPathItem>
{
public:
	enum ConnectionType
	{
		LineConnectionNormal,
		LineConnectionStartArrow,
		LineConnectionEndArrow,
		LineConnectionClosedStartArrow,
		LineConnectionClosedEndArrow,
		LineConnectionAggregation,
		LineConnectionComposition
	};
	struct MarkerInformation
	{
		MarkerInformation(const QPainterPath & path = QPainterPath(), bool fill = false)
			: _path(path),
			_fill(fill)
		{
		}

		QPainterPath _path;
		bool _fill;
	};
	enum { Type = UserType + 2};

public:
	GraphicsItemConnectionLine(GraphicsItemConnectionPoint * start, GraphicsItemConnectionPoint * end, QGraphicsItem * parent = 0);
	virtual ~GraphicsItemConnectionLine();

	void setStartType(ConnectionType type);
	void setEndType(ConnectionType type);

	void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual int type() const { return Type; }

	virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

private:
	void initialiseDefaults();
	void updatePosition();


	PtrVarGet(GraphicsItemConnectionPoint, startPoint);
	PtrVarGet(GraphicsItemConnectionPoint, endPoint);
	SimpleVarGet(ConnectionType, startType);
	SimpleVarGet(ConnectionType, endType);
	RefVarGetSet(QPen, selectedPen, SelectedPen);
	RefVarGetSet(QBrush, emptyBrush, EmptyBrush);

	MarkerInformation _startMarker;
	MarkerInformation _endMarker;
	float _middleStart, _middleEnd;

	static QMap<ConnectionType, MarkerInformation> _Markers;
	static void InitialiseMarkers();
};

#endif // GRAPHICSITEMCONNECTIONLINE_HPP
