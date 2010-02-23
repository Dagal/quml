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

#ifndef CLASSACTION_HPP
#define CLASSACTION_HPP

#include "iaction.hpp"
#include "macro.hpp"

class UMLDiagram;
class ElementObject;
class ClassObject;

class CreateClassAction : public IAction
{
public:
	CreateClassAction(const std::string & className, ElementObject * parent);

	virtual bool canExecute(std::string * errorMsg = 0) const;

private:
	virtual void performExecution();

	PtrVarGet(ElementObject, parentObject);
	PtrVarGet(ClassObject, classObject);
	RefVarGet(std::string, className);
};

class RenameClassAction : public IAction
{
public:
	RenameClassAction(ClassObject * classObject, const std::string & newName);

	virtual bool canExecute(std::string * errorMsg = 0) const;

private:
	virtual void performExecution();

private:
	PtrVarGet(ClassObject, classObject);
	RefVarGet(std::string, newName);
};

#endif // CLASSACTION_HPP
