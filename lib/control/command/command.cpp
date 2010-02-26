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
#include "_command.hpp"
#include "diagramcontroller.hpp"

Command::Command()
	: _dd(new CommandPrivate)
{
}

Command::~Command()
{
}

bool Command::initialise(DiagramController * controller)
{
	_dd->_controller = controller;
	_dd->_initError = checkAction();

	if(initError() != Error_NoError && _dd->_controller)
	{
		_dd->_controller->sendError(Event(Event_CommandInitialisation), initError());
		return false;
	}

	return true;
}

bool Command::redo()
{
	// is this command in a valid state?
	if(initError() != Error_NoError)
		return false;

	CommandPrivate::vctCommand executedCommands;

	for(unsigned int i = 0; i != _dd->_subCommands.size(); ++i)
	{
		Command * curCommand = _dd->_subCommands[i];

		// succesfully executed the command?
		if(curCommand->redo())
			executedCommands.push_back(curCommand);
		else
		{
			// perform cleanup
			for(int j = executedCommands.size(); j != 0; --j)
				executedCommands[j-1]->undo();

			return false;
		}
	}

	return redoAction();
}

bool Command::undo()
{
	// is this command in a valid state?
	if(initError() != Error_NoError)
		return false;

	// try to undo this command
	if(!undoAction())
		return false;

	bool succeeded = true;

	// and all the subcommands
	for(int i = _dd->_subCommands.size(); i != 0; --i)
	{
		Command * curCommand = _dd->_subCommands[i-1];
		succeeded &= curCommand->undo();
	}

	return succeeded;
}

void Command::sendEvent(const Event & event) const
{
	if(_dd->_controller)
		_dd->_controller->sendEvent(event);
}

void Command::sendError(const Event & event, Error error) const
{
	if(_dd->_controller)
		_dd->_controller->sendError(event, error);

}

Error Command::initError() const
{
	return _dd->_initError;
}
