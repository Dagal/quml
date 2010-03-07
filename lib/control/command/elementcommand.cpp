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

#include "elementcommand.hpp"
#include "classdiagramcontroller.hpp"
#include "elementobject.hpp"

DeleteElementCommand::DeleteElementCommand(ClassDiagramController * controller, const std::string & elementQualifiedName)
	: ICommand(Command_DetachElement),
	_controller(controller),
	_elementQualifiedName(elementQualifiedName),
	_element(0)
{
}

DeleteElementCommand::~DeleteElementCommand()
{
	if(_element && _element->umlDiagram() == 0)
		delete _element;
}

Error DeleteElementCommand::redo()
{
	_element = _controller->getElement(_elementQualifiedName);
	if(_element->parent())
		_parentQualifiedName = _element->parent()->qualifiedName();
	else
		_parentQualifiedName.clear();

	return _controller->detachElement(_elementQualifiedName);
}


Error DeleteElementCommand::undo()
{
	if(!_element)
		return Error_ElementUndefined;

	ElementObject * parent = _controller->getElement(_parentQualifiedName);
	_element->setParent(parent);
	_element->setUMLDiagram(_controller->diagram());

	_element = 0;

	return Error_NoError;
}

RenameElementCommand::RenameElementCommand(ClassDiagramController * controller, const std::string & elementQualifiedName, const std::string & newName)
	: ICommand(Command_RenameElement),
	_controller(controller),
	_elementOldQualifiedName(elementQualifiedName),
	_newName(newName)
{
}

Error RenameElementCommand::redo()
{
	ElementObject * element = _controller->getElement(elementOldQualifiedName());

	if(!element)
		return Error_ElementUndefined;

	_oldName = element->name();
	Error error = _controller->renameElement(elementOldQualifiedName(), newName());

	_elementNewQualifiedName = element->qualifiedName();

	return error;
}

Error RenameElementCommand::undo()
{
	return _controller->renameElement(elementNewQualifiedName(), oldName());
}

CreateElementCommand::CreateElementCommand(const std::string & elementName, const std::string & parentQualifiedName)
	: ICommand(Command_CreateElement),
	_elementName(elementName),
	_parentQualifiedName(parentQualifiedName),
	_elementObject(0)
{
}


MoveElementCommand::MoveElementCommand(const std::string & classQualifiedName, const std::string & parentNewQualifiedName)
	: ICommand(Command_MoveElement),
	_elementOldQualifiedName(classQualifiedName),
	_parentNewQualifiedName(parentNewQualifiedName)
{
}
