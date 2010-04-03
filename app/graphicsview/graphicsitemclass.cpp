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

GraphicsItemClass::GraphicsItemClass(ClassObject * classObject, QGraphicsItem * parent)
	: QGraphicsRectItem(parent),
	_dd(new GraphicsItemClass::GraphicsItemClassPrivate(classObject))
{
}

void GraphicsItemClass::updateItem()
{

}

ClassObject * GraphicsItemClass::classObject() const
{
	return _dd->classObject();
}

const QFont & GraphicsItemClass::font() const
{
	return _dd->font();
}
void GraphicsItemClass::setFont(const QFont & font)
{
	_dd->setFont(font);
}


QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateHeader(const QFontMetricsF & fontMetrics)
{
}

QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateProperties(const QFontMetricsF & fontMetrics)
{

}

QRectF GraphicsItemClass::GraphicsItemClassPrivate::calculateOperations(const QFontMetricsF & fontMetrics)
{

}
