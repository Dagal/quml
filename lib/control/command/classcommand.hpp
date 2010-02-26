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

#include "command.hpp"
#include "macro.hpp"
#include <string>


class ClassObject;
class ElementObject;
class UMLDiagram;

class CreateClassCommand : public Command
{
public:
	CreateClassCommand(const std::string & umlDiagramName, const std::string & className, const std::string & parentName);
	virtual ~CreateClassCommand();

private:
	virtual bool redoAction();
	virtual bool undoAction();
	virtual Error checkAction();

	const std::string _umlDiagramName;
	const std::string _className;
	const std::string _parentName;
	UMLDiagram * _umlDiagram;
	PtrVarGet(ClassObject, classObject);
	ElementObject * _parentObject;
	bool _isOwner;
};

class ChangeClassCommand : public Command
{
	ChangeClassCommand(const std::string & umlDiagramName, const std::string & className, const std::string & newName, const std::string & newUmlDiagram, const std::string & newParent);

private:
	virtual bool redoAction();
	virtual bool undoAction();
	virtual Error checkAction();

	const std::string _oldUmlDiagramName;
	const std::string _oldClassName;
	const std::string _newName;
	const std::string _newUmlDiagramName;
	const std::string _newParentName;

	UMLDiagram * _oldDiagram;
	UMLDiagram * _newDiagram;
	ClassObject * _classObject;
	ElementObject * _oldParentObject;
	ElementObject * _newParentObject;
};



#endif // CLASSCOMMAND_HPP

