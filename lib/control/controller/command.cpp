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

	SafeCommand::SafeCommand(const QList<UMLDiagramCommand*> & commands)
		: _commands(commands)
		, _nextIsRedo(true)
	{
	}

	SafeCommand::~SafeCommand()
	{
		qDeleteAll(_commands);
	}

	int SafeCommand::redo()
	{
		if(!_nextIsRedo) return Error_NotReadyForRedo;

		QSet<element::ElementObject*> _alteredElements;
		UMLDiagramCommand * problemCommand = 0;

		for(int i = 0; i < _commands.size(); i++)
		{
			UMLDiagramCommand * curCommand = _commands[i];

			// can we execute the command correctly?
			if(curCommand->redo() == Error_NoError)
			{
				// add all elements to check to the list
				_alteredElements.unite(curCommand->changedElements());
			}
			else
			{
				// store the problem command
				problemCommand = curCommand;

				// undo the loop
				undoLoop(i);

				// break the loop and send an error
				return Error_SubCommandError;
			}
		}

		// check all the altered elements


		// correctly executed
		_nextIsRedo = false;

		return 0;
	}

	void SafeCommand::undoLoop(int startPos)
	{
		for(int i = startPos; i >= 0; --i)
			_commands[i]->undo();
	}

	int SafeCommand::undo()
	{
		if(_nextIsRedo) return -1;
	}
}
