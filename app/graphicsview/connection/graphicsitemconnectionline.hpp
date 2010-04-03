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

namespace connection
{
	class GraphicsItemConnectionPoint;

	//! This class represents a line that connects two GraphicsItemConnectionPoints.
	/*!
		This class represents a line that connects two GraphicsItemConnectionPoints in the QGraphicsView Framework. This connection installs
		itemChangedListeners (GraphicsExt) in both points, so that the position changed messages are intercepted and the line can be updated.
		A line has a start and a end ConnectionType. The line is created by performing a translation and a rotation so that start lies at (0,0)
		and the end lies at (x,0) where x > 0.

		\todo Add static functions to register Marker information
	*/
	class GraphicsItemConnectionLine : public graphicsUtil::GraphicsExt<QGraphicsPathItem>
	{
	public:
		//! This enum specifies which type of connection is used.
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

		/*!
		  A MarkerInformation contains a path and and a boolean fill value. The QPainterPath should have as most left position (0,0) and should end
		  on the position(width, 0).
		*/
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
		/*!
		  This enum is defined to use the qgraphicsitem_cast functionality
		*/
		enum { Type = UserType + 2};

	public:
		GraphicsItemConnectionLine(GraphicsItemConnectionPoint * start, GraphicsItemConnectionPoint * end, QGraphicsItem * parent = 0);
		virtual ~GraphicsItemConnectionLine();

		void setStartType(ConnectionType type);
		void setEndType(ConnectionType type);

		void paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		virtual int type() const { return Type; }

		/*!
		  This function returns the startPoint for the line
		*/
		GraphicsItemConnectionPoint * startPoint() const { return _startPoint; }
		/*!
		  This function returns the endPoint for the line
		*/
		GraphicsItemConnectionPoint * endPoint() const { return _endPoint; }
		/*!
		  This function returns the ConnectionType for the start of the line
		*/
		ConnectionType startType() const { return _startType; }
		/*!
		  This function returns the ConnectionType for the end of the line
		*/
		ConnectionType endType() const { return _endType; }
		/*!
		  This function returns the pen used to draw when in selected mode
		*/
		const QPen & selectedPen() const { return _selectedPen; }
		/*!
		  This function returns the brush used when the ConnectionType, start or end, should not be filled
		*/
		const QBrush & emptyBrush() const { return _emptyBrush; }
		/*!
		  Use this function to set the pen that should be used when in selected mode
		*/
		void setSelectedPen(const QPen & selectedPen) { _selectedPen = selectedPen; }
		/*!
		  Use this function to set the brush that should be used to draw "unfilled" ConnectionType, start or end
		*/
		void setEmptyBrush(const QBrush & emptyBrush) { _emptyBrush = emptyBrush; }


	protected:
		virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

	private:
		void initialiseDefaults();
		void updatePosition();

		GraphicsItemConnectionPoint * _startPoint;
		GraphicsItemConnectionPoint * _endPoint;
		ConnectionType _startType;
		ConnectionType _endType;
		QPen _selectedPen;
		QBrush _emptyBrush;
		MarkerInformation _startMarker;
		MarkerInformation _endMarker;
		float _middleStart, _middleEnd;

		static QMap<ConnectionType, MarkerInformation> _Markers;
		static void InitialiseMarkers();
	};
}

#endif // GRAPHICSITEMCONNECTIONLINE_HPP
