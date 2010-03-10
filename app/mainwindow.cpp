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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "graphicsview/graphicsitemconnection.hpp"
#include "graphicsview/graphicsitemconnectionpoint.hpp"
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QGraphicsScene * scene = new QGraphicsScene(this);
	ui->mainView->setScene(scene);

	GraphicsItemConnection * c = new GraphicsItemConnection;
	GraphicsItemConnectionPoint * p1 = new GraphicsItemConnectionPoint(c);
	GraphicsItemConnectionPoint * p2 = new GraphicsItemConnectionPoint(c);
	GraphicsItemConnectionPoint * p3 = new GraphicsItemConnectionPoint(c);
	GraphicsItemConnectionPoint * p4 = new GraphicsItemConnectionPoint(c);

	p1->setPos(10,10);
	p2->setPos(50,20);
	p3->setPos(40,40);
	p4->setPos(70,70);

	QGraphicsPathItem path;
	QGraphicsPolygonItem poly;

	QGraphicsItem::GraphicsItemFlags flags =  path.flags() ^ poly.flags();

	for(int i = 1; i <= 0x400; i  = i << 1)
	{
		qDebug() << path.flags().testFlag((QGraphicsItem::GraphicsItemFlag)i) << poly.flags().testFlag((QGraphicsItem::GraphicsItemFlag)i);
	}


	scene->addItem(c);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
	switch (e->type())
	{
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
