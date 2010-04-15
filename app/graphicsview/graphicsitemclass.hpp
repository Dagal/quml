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

#ifndef GRAPHICSITEMCLASS_HPP
#define GRAPHICSITEMCLASS_HPP

#include <QGraphicsRectItem>
#include <QFont>
#include "classobject.hpp"

using element::ClassObject;

class GraphicsItemClass : public QGraphicsRectItem
{
	class GraphicsItemClassPrivate;

public:
	GraphicsItemClass(element::ClassObject * classObject, QGraphicsItem * parent = 0);

	element::ClassObject * classObject() const;
	const QFont & font() const;
	void setFont(const QFont & font);

	void updateItem();

	// overloaded member functions
	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	virtual QRectF boundingRect() const;

private:
	QSharedPointer<GraphicsItemClassPrivate> _dd;

};


#endif // GRAPHICSITEMCLASS_HPP
