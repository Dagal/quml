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

#ifndef GRAPHICSITEMCONNECTION_HPP
#define GRAPHICSITEMCONNECTION_HPP

#include <QGraphicsItem>
#include "../util/graphicsext.hpp"
#include "macro.hpp"

//! This namespace contains the classes for a simple connection, based in the QGraphicsViewFramework.
/*!
  The connection namespace contains the classes to create a simple connection based on the QGraphicsView Framework.
  \li GraphicsItemConnectionPoint: A simple point, that will define the start of the end of line
  \li GraphicsItemConnectionLine: A line that connects two points.
  \li GraphicsItemConnection: This class contains different points and add lines in between. It also has full control over the points and the lines.

  To create a own kind of line, inherit from GraphicsItemConnection and wanted methods. If you want to add new kind of points, just inherit
  from GraphicsItemConnectionPoint and pass on that new type to GraphicsItemConnection::attachPoint. If you want to add new kind of lines, also
  inherit from GraphicsItemConnectionLine and LineCreator, and set this new LineCreator using GraphicsItemConnection::setLineCreator.
*/
namespace connection
{
	class GraphicsItemConnectionPoint;
	class GraphicsItemConnectionLine;

	//! This functor is called when a line should be created between two points
	struct LineCreator
	{
		virtual ~LineCreator() {};

		/*!
		  Reimplement the method LineCreator::operator() and register and instance to the GraphicsItemConnection using
		  GraphicsItemConnection::setLineCreator.
		*/
		virtual GraphicsItemConnectionLine * operator()(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint) = 0;
	};

	//! This is the default functor when creating a GraphicsItemConnection
	struct DefaultLineCreator : public LineCreator
	{
		virtual GraphicsItemConnectionLine * operator()(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint);
	};

	//! This class contains different points and add lines in between. It also has full control over the points and the lines.
	/*!
	  This class contains different GraphicsItemConnectionPoint's and adds GraphicsItemConnectionLine's in between. By default, this class uses a two-phase
	  selection scheme. In first step the complete connection is selected, and afterwards the user can select a line or a point. Reimplement this class if
	  you want different behaviour.
	*/
	class GraphicsItemConnection : public graphicsUtil::GraphicsExt<QGraphicsItem>
	{
	public:
		GraphicsItemConnection(QGraphicsItem * parent = 0);
		~GraphicsItemConnection();

		virtual QRectF boundingRect() const { return _boundingRect; }
		virtual void paint(QPainter * , const QStyleOptionGraphicsItem * , QWidget * = 0) { };

		// point attachement functions
		void attachPoint(GraphicsItemConnectionPoint * point, int position = -1);
		GraphicsItemConnectionPoint *  detachPoint(int position);
		GraphicsItemConnectionPoint * pointAt(int position) const;
		int pointPosition(GraphicsItemConnectionPoint * point) const;

		/*!
		  Use this function to set the functor responsable for creating the lines between the points. GraphicsItemConnection takes control over
		  the LineCreator, so you don't have to delete it.
		*/
		void setLineCreator(LineCreator * lineCreator) { if(_lineCreator) detachLineCreator(); _lineCreator = lineCreator; }
		/*!
		  This function returns the functorto set the functor responsable for creating the lines between the points
		*/
		LineCreator * lineCreator() const { return _lineCreator; }
		/*!
		  This function detaches and destroys the attached LineCreator
		*/
		void detachLineCreator() { delete _lineCreator; _lineCreator = 0; }

	protected:
		virtual bool sceneEvent(QEvent * event);
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);
		virtual void focusInEvent(QFocusEvent * event);
		virtual void focusOutEvent(QFocusEvent * event);
		virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

		virtual void onUpdateConnection();
		virtual void onPointAdded(GraphicsItemConnectionPoint * newPoint);
		virtual void onPointRemoved(GraphicsItemConnectionPoint * oldPoint);
		virtual void onLineCreated(GraphicsItemConnectionLine * newLine);
		virtual void onLineDestroyed(GraphicsItemConnectionLine * oldLine);

	private:
		void updateCompleteConnection();
		void updateBoundingRect();

		GraphicsItemConnectionLine * createAndAttachLine(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint, int position);

	private:
		LineCreator * _lineCreator;
		QList<GraphicsItemConnectionLine *> _lines;
		QList<GraphicsItemConnectionPoint *> _points;
		QRectF _boundingRect;
	};
}


#endif // GRAPHICSITEMCONNECTION_HPP
