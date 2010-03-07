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
#include "macro.hpp"
#include <string>

class ClassDiagramController;
class ElementObject;

class DeleteElementCommand : public ICommand
{
public:
	DeleteElementCommand(ClassDiagramController * controller, const std::string & elementQualifiedName);
	~DeleteElementCommand();

	virtual Error redo();
	virtual Error undo();

private:
	ClassDiagramController * _controller;

	RefVarGet(std::string, elementQualifiedName);
	RefVarGet(std::string, parentQualifiedName);
	PtrVarGet(ElementObject, element);
};

class RenameElementCommand : public ICommand
{
public:
	RenameElementCommand(ClassDiagramController * controller, const std::string & elementQualifiedName, const std::string & newName);

	virtual Error redo();
	virtual Error undo();

private:
	ClassDiagramController * _controller;

	RefVarGet(std::string, elementOldQualifiedName);
	RefVarGet(std::string, elementNewQualifiedName);
	RefVarGet(std::string, oldName);
	RefVarGet(std::string, newName);
};

class CreateElementCommand : public ICommand
{
protected:
	CreateElementCommand(const std::string & elementName, const std::string & parentQualifiedName);

private:
	RefVarGetProtectedAcc(std::string, elementName);
	RefVarGetProtectedAcc(std::string, parentQualifiedName);
	PtrVarGetProtectedAcc(ElementObject, elementObject);
};


class MoveElementCommand : public ICommand
{
protected:
	MoveElementCommand(const std::string & elementQualifiedName, const std::string & parentNewQualifiedName);

private:
	RefVarGetProtectedAcc(std::string, elementOldQualifiedName);
	RefVarGetProtectedAcc(std::string, elementNewQualifiedName);
	RefVarGetProtectedAcc(std::string, parentNewQualifiedName);
	RefVarGetProtectedAcc(std::string, parentOldQualifiedName);
};


#endif // ELEMENTCOMMAND_HPP
