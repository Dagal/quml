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

#include "command.hpp"
#include "umldiagram.hpp"
#include "classdiagramcontroller.hpp"

using namespace element;

namespace controller
{
	RenameCommand::RenameCommand(UMLDiagram * diagram, const QString & elementQualifiedName, const QString & newName)
		: UMLDiagramCommand(diagram)
		, _oldQualifiedName(elementQualifiedName)
		, _newName(newName)
	{
	}

	int RenameCommand::redo()
	{
		// find the element
		ElementObject * element = diagram()->findElement(oldQualifiedName());
		if(element == 0) return Error_ElementNotFound;

		// perform operations and make sure undo is possible
		_oldName = element->name();
		element->setName(newName());
		_newQualifiedName = element->qualifiedName();

		// store the changed elements
		_changedElements << element;

		return Error_NoError;
	}

	int RenameCommand::undo()
	{
		// find the element
		ElementObject * element = diagram()->findElement(oldQualifiedName());
		if(element == 0) return Error_ElementNotFound;

		// perform operations
		element->setName(oldName());

		return Error_NoError;
	}

	MoveCommand::MoveCommand(UMLDiagram * diagram, const QString & elementQualifiedName, const QString & newParentQualifiedName)
		: UMLDiagramCommand(diagram)
		, _oldQualifiedName(elementQualifiedName)
		, _newParentQualifiedName(newParentQualifiedName)
	{
	}

	int MoveCommand::redo()
	{
		// find the element
		ElementObject * element = diagram()->findElement(oldQualifiedName());
		if(element == 0) return Error_ElementNotFound;

		// perform operations and make sure undo is possible
		ElementObject * oldParent = element->parent();
		_oldParentQualifiedName = oldParent ? element->parent()->qualifiedName() : "";

		ElementObject * newParent = diagram()->findElement(newParentQualifiedName());
		element->setParent(newParent);
		_newQualifiedName = element->qualifiedName();

		// store the changed elements
		_changedElements << element << oldParent << newParent;

		return Error_NoError;
	}

	int MoveCommand::undo()
	{
		// find the element
		ElementObject * element = diagram()->findElement(newQualifiedName());
		if(element == 0) return Error_ElementNotFound;

		// perform operations
		element->setParent(diagram()->findElement(oldParentQualifiedName()));

		return Error_NoError;
	}

	CheckElementsCommand::CheckElementsCommand(const QSet<element::ElementObject *> & elements)
		: _elements(elements)
	{
		undo();
	}

	void CheckElementsCommand::reinitialise(const QSet<element::ElementObject *> & elements)
	{
		undo();
		_elements = elements;
	}

	int CheckElementsCommand::redo()
	{
		undo();

		ClassDiagramController * controller = ClassDiagramController::Instance();

		// check all elements
		foreach(element::ElementObject * element, _elements)
		{
			const IClassDiagramRules * rules = controller->RulesFor(element);
			_error.errorCode = rules->checkElement(element);

			if(hasError())
			{
				_error.type = element->type();
				_error.elementQualifiedName = element->qualifiedName();
				break;
			}
		}

		return _error.errorCode;
	}

	int CheckElementsCommand::undo()
	{
		_error.errorCode = Error_NoError;
		_error.type = Element;
		_error.elementQualifiedName = QString();

		return _error.errorCode;
	}
}
