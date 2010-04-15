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

#ifndef ELEMENTCOMMAND_HPP
#define ELEMENTCOMMAND_HPP

#include "icommand.hpp"
#include "element.hpp"
#include <QString>

class ClassDiagramController;

class DeleteElementCommand : public ICommand
{
public:
	DeleteElementCommand(ClassDiagramController * controller, const QString & elementQualifiedName);
	~DeleteElementCommand();

	virtual bool redo();
	virtual bool undo();

private:
	ClassDiagramController * _controller;
	QString _elementQualifiedName;
	QString _parentQualifiedName;
	element::ElementObject * _element;
};


class RenameElementCommand : public ICommand
{
public:
	RenameElementCommand(ClassDiagramController * controller, const QString & elementQualifiedName, const QString & newName);

	virtual bool redo();
	virtual bool undo();

private:
	ClassDiagramController * _controller;
	QString _oldElementQualifiedName;
	QString _newElementQualifiedName;
	QString _oldName;
	QString _newName;
};

#endif // ELEMENTCOMMAND_HPP
