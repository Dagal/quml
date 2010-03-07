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
#include <QFontMetricsF>
#include <QPainter>
#include "classobject.hpp"
#include "propertyobject.hpp"
#include "operationobject.hpp"

const qreal GraphicsItemClass::LINE_SKIP = 1.0f;
const qreal GraphicsItemClass::BORDER_SKIP = 2.0f;
QFont GraphicsItemClass::_DefaultFont = QFont();

GraphicsItemClass::GraphicsItemClass(ClassObject * classObject)
	: _classObject(classObject)
{
	calculateBoundingRect();
}

QRectF GraphicsItemClass::boundingRect() const
{
	return _boundingRect;
}

void GraphicsItemClass::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if(_classObject == 0)
		return;

	QFontMetricsF fm(DefaultFont());
	painter->setFont(DefaultFont());
	qreal textHeight = fm.height();

	QPointF startCo(-boundingRect().left(), -boundingRect().top());
	qreal startX = -boundingRect().left() + BORDER_SKIP;
	qreal startY = -boundingRect().top() + fm.ascent() + BORDER_SKIP;

	// check the class Name
	QString className = QString::fromStdString(_classObject->umlName());
	painter->drawText(startX, startY, className);

	// draw the first box
	painter->drawRect(startCo.x(), startCo.y(), boundingRect().width(), textHeight + BORDER_SKIP*2);

//
//
//	qreal heigth = fm.width(className);
//	height += LINE_SKIP + textHeight;
//
//	// check all the properties
//	const std::vector<PropertyObject*> & properties = _classObject->properties();
//	for(int i = 0; i < properties.size(); i++)
//	{
//		PropertyObject * curProperty = properties[i];
//
//		qreal curWidth = fm.width(QString::fromStdString(curProperty->umlName()));
//		if(curWidth > width)
//			width = curWidth;
//
//		height += LINE_SKIP + textHeight;
//	}
//
//	// check all the operations
//	const std::vector<OperationObject*> & operations = _classObject->operations();
//	for(int i = 0; i < operations.size(); i++)
//	{
//		OperationObject * curOperation = operations[i];
//
//		qreal curWidth = fm.width(QString::fromStdString(curOperation->umlName()));
//		if(curWidth > width)
//			width = curWidth;
//
//		height += LINE_SKIP + textHeight;
//	}
//
}

void GraphicsItemClass::calculateBoundingRect()
{
	QFontMetricsF fm(DefaultFont());

	_boundingRect = QRectF();

	qreal textHeight = fm.height();

	if(_classObject == 0)
		return;

	qreal height = BORDER_SKIP*6;

	// check the class Name
	qreal width = fm.width(QString::fromStdString(_classObject->umlName()));
	height += textHeight;

	// check all the properties
	const std::vector<PropertyObject*> & properties = _classObject->properties();
	for(int i = 0; i < properties.size(); i++)
	{
		PropertyObject * curProperty = properties[i];

		qreal curWidth = fm.width(QString::fromStdString(curProperty->umlName()));
		if(curWidth > width)
			width = curWidth;

		height += LINE_SKIP + textHeight;
	}

	// check all the operations
	const std::vector<OperationObject*> & operations = _classObject->operations();
	for(int i = 0; i < operations.size(); i++)
	{
		OperationObject * curOperation = operations[i];

		qreal curWidth = fm.width(QString::fromStdString(curOperation->umlName()));
		if(curWidth > width)
			width = curWidth;

		height += LINE_SKIP + textHeight;
	}


	width += (BORDER_SKIP*2);
	height -= (LINE_SKIP*2);

	// update the bounding rect
	_boundingRect = QRectF(-width/2, -height/2, width, height);

}
