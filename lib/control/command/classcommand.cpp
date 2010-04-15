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

using namespace element;

CreateClassCommand::CreateClassCommand(ClassDiagramController * controller, const QString & className, const QString & parentQualifiedName)
	: _controller(controller),
	_className(className),
	_parentQualifiedName(parentQualifiedName),
	_classObject(0)
{
}

bool CreateClassCommand::redo()
{
	_classObject = 0;
	return _controller->createClass(className(), parentQualifiedName(), &_classObject) == Error_NoError;
}

bool CreateClassCommand::undo()
{
	delete _classObject;
	_classObject = 0;

	return true;
}


MoveClassCommand::MoveClassCommand(ClassDiagramController * controller, const QString & oldclassQualifiedName, const QString & newparentQualifiedName)
	: _controller(controller),
	_oldclassQualifiedName(oldclassQualifiedName),
	_newparentQualifiedName(newparentQualifiedName)
{
}

bool MoveClassCommand::redo()
{
	ClassObject * classObject = _controller->getElement<ClassObject>(oldclassQualifiedName());

	if(classObject == 0)
		return false;

	if(classObject->parent())
		_oldparentQualifiedName = classObject->parent()->qualifiedName();

	Error error = _controller->moveClass(oldclassQualifiedName(), newparentQualifiedName());

	_newclassQualifiedName = classObject->qualifiedName();

	return error == Error_NoError;
}

bool MoveClassCommand::undo()
{
	return _controller->moveClass(_newclassQualifiedName, _oldparentQualifiedName) == Error_NoError;
}
