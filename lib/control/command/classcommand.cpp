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
#include "controller.hpp"
#include "classdiagramcontroller.hpp"
#include "event.hpp"
#include "_command.hpp"
#include "umldiagram.hpp"
#include "classobject.hpp"

CreateClassCommand::CreateClassCommand(const std::string & umlDiagramName, const std::string & className, const std::string & parentName)
	:_umlDiagramName(umlDiagramName),
	_className(className),
	_parentName(parentName),
	_umlDiagram(0),
	_classObject(0),
	_parentObject(0),
	_isOwner(false)
{
}


CreateClassCommand::~CreateClassCommand()
{
	if(_isOwner)
		delete _classObject;
}


bool CreateClassCommand::redoAction()
{
	_isOwner = false;
	if(_classObject == 0)
		_classObject = new ClassObject;

	_classObject->setName(_className);
	_classObject->setParent(_parentObject);
	_classObject->setUMLDiagram(_umlDiagram);

	return true;
}

bool CreateClassCommand::undoAction()
{
	_isOwner = true;

	_classObject->setParent(0);
	_classObject->setUMLDiagram(0);
	_classObject->setName("");

	return true;
}

Error CreateClassCommand::checkAction()
{
	// get & check the diagram
	_umlDiagram = Controller::Instance()->classDiagramController()->findUMLDiagram(_umlDiagramName);
	if(_umlDiagram == 0) return Error_UndefinedUMLDiagram;

	// get & check the parent
	_parentObject = _umlDiagram->findElement(_parentName);
	if(_parentObject != 0 && _parentObject->type() != Element_Class && _parentObject->type() != Element_Package) return Error_ClassParentBadContainer;

	// see if we can add a class with this name
	if(_umlDiagram->findElement<ElementObject>(_className, _parentObject) != 0) return  Error_ClassNameAlreadyUsed;

	return Error_NoError;
}

ChangeClassCommand::ChangeClassCommand(const std::string & umlDiagramName, const std::string & className, const std::string & newName, const std::string & newUmlDiagram, const std::string & newParent)
	: _oldUmlDiagramName(umlDiagramName),
	_oldClassName(className),
	_newName(newName),
	_newUmlDiagramName(newUmlDiagram),
	_newParentName(newParent),
	_oldDiagram(0),
	_newDiagram(0),
	_classObject(0),
	_oldParentObject(0),
	_newParentObject(0)
{
}

bool ChangeClassCommand::redoAction()
{
	// should we change from umlDiagram?
	if(_oldDiagram != _newDiagram) _classObject->setUMLDiagram(0);

	// should we change from parents?
	if(_oldParentObject != _newParentObject) _classObject->setParent(0);

	// change the name
	_classObject->setName(_newName);

	// reattach the object
	_classObject->setParent(_newParentObject);
	_classObject->setUMLDiagram(_newDiagram);

	return true;
}

bool ChangeClassCommand::undoAction()
{
	// should we change from umlDiagram?
	if(_oldDiagram != _newDiagram) _classObject->setUMLDiagram(0);

	// should we change from parents?
	if(_oldParentObject != _newParentObject) _classObject->setParent(0);

	// change the name
	_classObject->setName(_oldClassName);

	// reattach the object
	_classObject->setParent(_oldParentObject);
	_classObject->setUMLDiagram(_oldDiagram);

	return true;
}

Error ChangeClassCommand::checkAction()
{
	// get & check the diagram
	_oldDiagram = Controller::Instance()->classDiagramController()->findUMLDiagram(_oldUmlDiagramName);
	if(_oldDiagram == 0) return Error_UndefinedUMLDiagram;

	// get the element
	_classObject = _oldDiagram->findElement<ClassObject>(_oldClassName, 0);
	if(_classObject == 0) return Error_ClassUndefined;

	// get & check the new diagram
	if(_newUmlDiagramName.empty() || _newUmlDiagramName == _oldUmlDiagramName)
		_newDiagram = _oldDiagram;
	else
		_newDiagram = Controller::Instance()->classDiagramController()->findUMLDiagram(_newUmlDiagramName);
	if(_newDiagram == 0) return Error_UndefinedUMLDiagram;

	// get & check the new parent
	_newParentObject = _newDiagram->findElement(_newParentName);
	if(_newParentObject != 0 && _newParentObject->type() != Element_Class && _newParentObject->type() != Element_Package) return Error_ClassParentBadContainer;

	return Error_NoError;
}
