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

#include "classaction.hpp"
#include "_action.hpp"
#include <boost/bind.hpp>
#include "elementobject.hpp"
#include "classobject.hpp"
#include "elementhelper.hpp"
#include <cassert>

CreateClassAction::CreateClassAction(const std::string & className, ElementObject * parent)
	: _parentObject(parent), _classObject(0), _className(className)
{
}


bool CreateClassAction::canExecute(std::string * errorMsg) const
{
	if(!checkCondition(className().empty(), errorMsg, "No className specified")) return false;
	if(!checkCondition(parentObject() == 0, errorMsg, "No parent element specified")) return false;

	bool parentIsRightContainer = (parentObject()->type() == Element_Class) || (parentObject()->type() == Element_Package);
	if(!checkCondition(parentIsRightContainer, errorMsg, "No valid parent specified")) return false;

	bool parentHasElementWithThisName = findChildren(parentObject(), className()).size() == 0;
	if(!checkCondition(parentHasElementWithThisName, errorMsg, "The parent already contains an element with this name")) return false;

	return true;
}

void CreateClassAction::performExecution()
{
	_classObject = new ClassObject;
	_classObject->setName(className());
	_classObject->setParent(parentObject());
}

RenameClassAction::RenameClassAction(ClassObject * classObject, const std::string & newName)
	: _classObject(classObject), _newName(newName)
{
}

bool RenameClassAction::canExecute(std::string * errorMsg) const
{
	if(!checkCondition(classObject() == 0, errorMsg, "No class specified")) return false;
	if(!checkCondition(newName().empty(), errorMsg, "No name specified")) return false;

	bool parentHasElementWithThisName =
			( classObject()->parent() ) &&
			( findChildren(classObject()->parent(), newName()).size() != 0 );
	if(!checkCondition(parentHasElementWithThisName, errorMsg, "The parent already has an element with this name")) return false;

	return true;
}

void RenameClassAction::performExecution()
{
	_classObject->setName(newName());
}
