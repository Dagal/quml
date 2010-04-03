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

#ifndef ACTION_HPP
#define ACTION_HPP

#include "element.hpp"
#include "macro.hpp"
#include "error.hpp"
#include <string>

enum ActionType
{
	Action_CreateElement,
	Action_DetachElement,
	Action_MoveElement,
	Action_RenameElement,
};

struct Action
{
	Action(ActionType type) : elementObject(0), error(Error_NoError), _type(type) {}
	ActionType type() const { return _type; }

	element::ElementObject * elementObject;
	Error error;

private:
	ActionType _type;
};

struct MoveAction : public Action
{
	MoveAction() : Action(Action_MoveElement), oldParent(0) {}

	element::ElementObject * oldParent;
};

struct RenameAction : public Action
{
	RenameAction() : Action(Action_RenameElement), oldName("") {}

	std::string oldName;
};

struct CreateAction : public Action
{
	CreateAction() : Action(Action_CreateElement) {}
};

struct DetachAction : public Action
{
	DetachAction() : Action(Action_DetachElement), oldParent(0) {}

	element::ElementObject * oldParent;
};


#endif // ACTION_HPP
