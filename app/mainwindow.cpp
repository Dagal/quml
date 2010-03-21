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
#include "graphicsview/graphicsitemconnectionline.hpp"
#include "graphicsview/graphicsitemconnectionpoint.hpp"
#include "graphicsview/graphicsitemconnection.hpp"
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <QDebug>
#include "test.hpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QGraphicsScene * scene = new QGraphicsScene(this);
	ui->mainView->setScene(scene);
	ui->mainView->setTransform(QTransform(), false);

//	Parent * p = new Parent();
//	scene->addItem(p);
//	Child * c1 = new Child(p);
//	Child * c2 = new Child(p);
//
//	c1->setPos(0,0);
//	c2->setPos(20,0);


	GraphicsItemConnection * connection = new GraphicsItemConnection(0);
	scene->addItem(connection);

	connection->createPoint(-1, QPointF(-50,40));
	connection->createPoint(-1, QPointF(70,-30));
	connection->createPoint(-1, QPointF(100,100));
	connection->createPoint(-1, QPointF(130,100));
	connection->createPoint(-1, QPointF(100,140));
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
