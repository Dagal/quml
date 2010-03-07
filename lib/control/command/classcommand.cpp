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

#include "classcommand.hpp"
#include "classdiagramcontroller.hpp"
#include "classobject.hpp"

CreateClassCommand::CreateClassCommand(ClassDiagramController * controller, const std::string & className, const std::string & parentQualifiedName)
	: CreateElementCommand(className, parentQualifiedName),
	_controller(controller)
{
}

Error CreateClassCommand::redo()
{
	elementObject() = 0;
	ClassObject * classObject = 0;

	Error error = _controller->createClass(elementName(), parentQualifiedName(), &classObject);
	elementObject() = classObject;

	return error;
}

Error CreateClassCommand::undo()
{
	delete elementObject();
	elementObject() = 0;

	return Error_NoError;
}

ClassObject * CreateClassCommand::classObject() const
{
	return element_cast<ClassObject>(elementObject());
}

MoveClassCommand::MoveClassCommand(ClassDiagramController * controller, const std::string & classQualifiedName, const std::string & parentNewQualifiedName)
	: MoveElementCommand(classQualifiedName, parentNewQualifiedName),
	_controller(controller)
{
}

Error MoveClassCommand::redo()
{
	ClassObject * classObject = _controller->getElement<ClassObject>(elementOldQualifiedName());

	if(classObject == 0)
		return Error_ElementUndefined;

	if(classObject->parent())
		parentOldQualifiedName() = classObject->parent()->qualifiedName();

	Error error = _controller->moveClass(elementOldQualifiedName(), parentNewQualifiedName());

	elementNewQualifiedName() = classObject->qualifiedName();

	return error;
}

Error MoveClassCommand::undo()
{
	return _controller->moveClass(elementNewQualifiedName(), parentOldQualifiedName());
}

