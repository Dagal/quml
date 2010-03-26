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
#include "../util/iextendedgraphicsitem.hpp"
#include "macro.hpp"

class GraphicsItemConnectionPoint;
class GraphicsItemConnectionLine;

struct LineCreator
{
	virtual ~LineCreator() {};

	virtual GraphicsItemConnectionLine * operator()(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint) = 0;
};
struct DefaultLineCreator : public LineCreator
{
	virtual GraphicsItemConnectionLine * operator()(GraphicsItemConnectionPoint * startPoint, GraphicsItemConnectionPoint * endPoint);
};

class GraphicsItemConnection : public QGraphicsItem, public ItemChangedListener
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

	virtual bool itemChangedFilter(QGraphicsItem * item, QGraphicsItem::GraphicsItemChange change, const QVariant & inValue, QVariant & outValue);

protected:
	virtual bool sceneEvent(QEvent * event);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);
	virtual void focusInEvent(QFocusEvent * event);
	virtual void focusOutEvent(QFocusEvent * event);

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
	PtrVarGetSet(LineCreator, lineCreator, LineCreator);
	QList<GraphicsItemConnectionLine *> _lines;
	QList<GraphicsItemConnectionPoint *> _points;
	QRectF _boundingRect;
};


#endif // GRAPHICSITEMCONNECTION_HPP
