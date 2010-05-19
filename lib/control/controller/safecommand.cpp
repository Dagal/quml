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

namespace controller
{
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
