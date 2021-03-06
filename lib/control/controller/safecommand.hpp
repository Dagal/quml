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

#ifndef SAFECOMMAND_HPP
#define SAFECOMMAND_HPP

#include "icommand.hpp"
#include "command.hpp"
#include <QList>
#include <boost/shared_ptr.hpp>

namespace controller
{
	class SafeCommand : public ICommand
	{
		class SafeCommandPrivate;

	public:
		struct CommandError
		{
			CommandError()
				: command(0)
				, errorCode(Error_NoError)
			{
			}

			ICommand * command;
			int errorCode;
		};

	public:
		SafeCommand(const QList<UMLDiagramCommand*> & commands);

		virtual int redo();
		virtual int undo();

		const QList<UMLDiagramCommand*> & commands() const;
		const CommandError & error() const;

	private:
		boost::shared_ptr<SafeCommandPrivate> _dd;
	};
}

#endif // SAFECOMMAND_HPP
