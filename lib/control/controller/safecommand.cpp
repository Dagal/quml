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

#include "safecommand.hpp"
#include "_safecommand.hpp"
#include "classdiagramcontroller.hpp"

namespace controller
{
	SafeCommand::SafeCommand(const QList<UMLDiagramCommand*> & commands)
		: _dd(new SafeCommandPrivate(commands))
	{
	}

	int SafeCommand::redo()
	{
		if(!_dd->_nextIsRedo) return Error_NotReadyForRedo;

		// clean the error details
		_dd->setError(0, Error_NoError);

		QSet<element::ElementObject*> alteredElements;

		// redo all the commands
		for(int i = 0; i < commands().size(); ++i)
		{
			UMLDiagramCommand * curCommand = commands()[i];
			int curError = curCommand->redo();

			// error in command, cleanup and exit
			if(curError != Error_NoError)
			{
				_dd->setError(curCommand, curError);
				_dd->undoLoop(i);

				return Error_SubCommandError;
			}

			// store the changed elements to check
			alteredElements.unite(curCommand->changedElements());
		}

		// check all the elements and unroll if necessary
		_dd->_checker = boost::shared_ptr<CheckElementsCommand>(new CheckElementsCommand(alteredElements));
		if(_dd->_checker->redo() != Error_CheckingElementsError)
		{
			_dd->setError(_dd->_checker.get(), Error_CheckingElementsError);
			_dd->undoLoop(commands().size()-1);

			return Error_CheckingElementsError;
		}

		// everything is okay
		_dd->_nextIsRedo = false;

		return Error_NoError;
	}

	int SafeCommand::undo()
	{
		if(_dd->_nextIsRedo) return Error_NotReadyForUndo;

		// clean all the error details
		_dd->setError(0, Error_NoError);

		// try to undo everything
		for(int i = commands().size()-1; i >= 0; --i)
		{
			UMLDiagramCommand * curCommand = commands()[i];
			int curError = curCommand->undo();

			if(curError != Error_NoError)
			{
				_dd->setError(curCommand, curError);
				return Error_SubCommandError;
			}
		}

		_dd->_nextIsRedo = true;
		return Error_NoError;

	}

	void SafeCommand::SafeCommandPrivate::undoLoop(int startPos)
	{
		for(int i = startPos; i >= 0; --i)
			_commands[i]->undo();
	}

	const QList<UMLDiagramCommand*> & SafeCommand::commands() const
	{
		return _dd->_commands;
	}

	const SafeCommand::CommandError & SafeCommand::error() const
	{
		return _dd->_commandError;
	}

	void SafeCommand::SafeCommandPrivate::setError(ICommand * command, int errorCode)
	{
		_commandError.command = command;
		_commandError.errorCode = errorCode;
	}
}
