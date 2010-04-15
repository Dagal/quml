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

#include "graphicsitemclass.hpp"
#include "_graphicsitemclass.hpp"
#include <propertyobject.hpp>
#include <QPainter>

using namespace element;

const float lineSkip = 2.0f;
const float boxSkip = 2.0f;
const float lineWidth = 1.0f;


GraphicsItemClass::GraphicsItemClass(ClassObject * classObject, QGraphicsItem * parent)
	: QGraphicsRectItem(parent),
	_dd(new GraphicsItemClass::GraphicsItemClassPrivate(classObject))
{
	updateItem();
}

void GraphicsItemClass::updateItem()
{
	_dd->_boundingRect = _dd->calculateHeader(QFontMetricsF(_dd->_font));
}

ClassObject * GraphicsItemClass::classObject() const
{
	return _dd->_classObject;
}

const QFont & GraphicsItemClass::font() const
{
	return _dd->_font;
}
void GraphicsItemClass::setFont(const QFont & font)
{
	_dd->_font = font;
}


/*!
  This methods updates the boundingRect by adding a new line of text (size calculated with fontMetrics) underneath the box
*/
void updateRectWithNewLine(QRectF & boundingRect, const QFontMetricsF & fontMetrics, const QString & text, float lineSkip)
{
	QRectF txtRct = fontMetrics.boundingRect(text);

	boundingRect.setLeft(qMin(boundingRect.left(), txtRct.left()));
	boundingRect.setRight(qMax(boundingRect.right(), txtRct.right()));

	boundingRect.setBottom(boundingRect.bottom() + fontMetrics.height() + lineSkip);
}

/*!
  inline template helper method for calculating the size of a box
*/
template <class T> inline QRectF calculateBoxRect(const QFontMetricsF & fontMetrics, const QList<T*> & elements)
{
	QRectF boundingRect;

	foreach(T * element, elements)
		updateRectWithNewLine(boundingRect, fontMetrics, element->umlName(), lineSkip);

	return boundingRect;
}

/*!
  A simple helper method that calculates the size of header for the UML drawing for a class
*/
QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateHeader(const QFontMetricsF & fontMetrics)
{
	return fontMetrics.boundingRect(_classObject->umlName());
}

/*!
  A simple helper method that calculates the size of properties-box for the UML drawing for a class
*/
QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateProperties(const QFontMetricsF & fontMetrics)
{
	return calculateBoxRect(fontMetrics, _classObject->properties());
}

/*!
  A simple helper method that calculates the size of operations-box for the UML drawing for a class
*/
QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateOperations(const QFontMetricsF & fontMetrics)
{
	return calculateBoxRect(fontMetrics, _classObject->properties());
}

void GraphicsItemClass::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawRect(boundingRect());
}

QRectF GraphicsItemClass::boundingRect() const
{
	return _dd->_boundingRect;
}
