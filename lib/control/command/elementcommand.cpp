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
#include "elementobject.hpp"
#include "umldiagram.hpp"
#include "classdiagramcontroller.hpp"
#include "controller.hpp"

DeleteElementCommand::DeleteElementCommand(const std::string & umlDiagram, const std::string & elementName)
	: _elementObject(0),
	_parentObject(0),
	_umlDiagram(0),
	_umlDiagramName(umlDiagram),
	_elementName(elementName),
	_isOwner(false)

{
}

DeleteElementCommand::~DeleteElementCommand()
{
	if(_isOwner) delete _elementObject;
}


bool DeleteElementCommand::redoAction()
{
	_isOwner = true;
	_parentObject = _elementObject->parent();

	_elementObject->setUMLDiagram(0);
	_elementObject->setParent(0);

	return true;
}

bool DeleteElementCommand::undoAction()
{
	_elementObject->setUMLDiagram(_umlDiagram);
	_elementObject->setParent(_parentObject);

	return true;
}

Error DeleteElementCommand::checkAction()
{
	_umlDiagram = Controller::Instance()->classDiagramController()->findUMLDiagram(_umlDiagramName);
	if(!_umlDiagram) return Error_UndefinedUMLDiagram;

	_elementObject = _umlDiagram->findElement(_elementName);
	if(!_elementObject) return Error_ClassUndefined;

	return Error_NoError;
}

