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

#ifndef GRAPHICSITEMRELATION_HPP
#define GRAPHICSITEMRELATION_HPP

#include "connection/graphicsitemconnection.hpp"

class GraphicsItemRelation : public connection::GraphicsItemConnection
{
public:
	GraphicsItemRelation(QGraphicsItem * parent = 0);

protected:
	virtual bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);

	virtual void onLineCreated(connection::GraphicsItemConnectionLine * newLine);
	virtual void onPointAdded(connection::GraphicsItemConnectionPoint * newPoint);
	virtual void onPointRemoved(connection::GraphicsItemConnectionPoint * oldPoint);


private:
	bool onPointContextMenu(connection::GraphicsItemConnectionPoint * point, QGraphicsSceneContextMenuEvent * event);
	bool onLineContextMenu(connection::GraphicsItemConnectionLine * line, QGraphicsSceneContextMenuEvent * event);
	bool onLineDoubleClick(connection::GraphicsItemConnectionLine * line, QGraphicsSceneMouseEvent * event);

};

#endif // GRAPHICSITEMRELATION_HPP
