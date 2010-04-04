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

#ifndef GRAPHICSITEMCONNECTIONPOINT_HPP
#define GRAPHICSITEMCONNECTIONPOINT_HPP

#include <QGraphicsItem>
#include <QSet>
#include <QPen>
#include <QBrush>
#include "../util/graphicsext.hpp"
#include "macro.hpp"

namespace connection
{
	class GraphicsItemConnectionLine;

	//! This class represents a simple point used for creating lines and connections
	/*!
		This class represents the point used for creating lines and connections and is based on the QGraphicsView Framework. A point has three states:
		disabled, visible and selected. The state defines whether the point is visible (PointVisible and PointSelected) or movable(PointSelected).

		A point has 2 brushes and 2 pens:
			\li GraphicsItemConnectionPoint::pen and GraphicsItemConnectionPoint::brush for drawing in PointVisible mode
			\li GraphicsItemConnectionPoint::selectedPen and GraphicsItemConnectionPoint::selectedBrush  for drawing in PointSelected mode
		in PointDisabled mode, the point will not be drawn.
	*/
	class GraphicsItemConnectionPoint : public graphicsUtil::GraphicsExt<QGraphicsRectItem>
	{
	public:
		/*!
		  This enum is defined to use the qgraphicsitem_cast functionality
		*/
		enum { Type = UserType + 1};

		/*!
		  This enum defines which state the point is in. If the point is used in a GraphicsItemConnection, the connection will set the right state. Otherwise
		  the user is responsable for setting the right state
		*/
		enum PointStatus { PointDisabled /*!< The point is not visible and not movable*/,
						   PointVisible /*!< The point is visible and can be selected*/,
						   PointSelected /*!< The point is selected, receives input and can be moved*/};

		GraphicsItemConnectionPoint(QGraphicsItem * parent = 0);

		void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
		virtual int type() const { return Type; }

		/*!
		  This function returns the pen used for drawing in PointSelected state
		*/
		const QPen & selectedPen() const { return _selectedPen; }
		/*!
		  This function returns the brush used for drawing in PointSelected state
		*/
		const QBrush & selectedBrush() const { return _selectedBrush; }
		/*!
		  This function returns the current state.
			\sa PointStatus
		*/
		PointStatus pointStatus() const { return _pointStatus; }
		/*!
		  This function sets the pen used for drawing in PointSelected state
		*/
		void setSelectedPen(const QPen & pen) { _selectedPen = pen; }
		/*!
		  This function sets the brush used for drawing in PointSelected state
		*/
		void setSelectedBrush(const QBrush & brush) { _selectedBrush = brush; }
		void setPointStatus(PointStatus  pointStatus);

	protected:
		virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value);

	private:
		QPen _selectedPen;
		QBrush _selectedBrush;
		PointStatus _pointStatus;
		QSet<GraphicsItemConnectionLine *> _lines;
	};



}





#endif // GRAPHICSITEMCONNECTIONPOINT_HPP
