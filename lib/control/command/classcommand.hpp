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

#ifndef CLASSCOMMAND_HPP
#define CLASSCOMMAND_HPP

#include "icommand.hpp"
#include "macro.hpp"
#include "elementcommand.hpp"
#include <string>

class ClassObject;
class ClassDiagramController;

class CreateClassCommand : public CreateElementCommand
{
public:
	CreateClassCommand(ClassDiagramController * controller, const std::string & className, const std::string & parentQualifiedName);

	virtual Error redo();
	virtual Error undo();

	ClassObject * classObject() const;

private:
	ClassDiagramController * _controller;
};

class MoveClassCommand : public MoveElementCommand
{
public:
	MoveClassCommand(ClassDiagramController * controller, const std::string & oldclassQualifiedName, const std::string & newparentQualifiedName);

	virtual Error redo();
	virtual Error undo();

private:
	ClassDiagramController * _controller;
};

#endif // CLASSCOMMAND_HPP
