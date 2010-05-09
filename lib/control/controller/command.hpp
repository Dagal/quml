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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "icommand.hpp"
#include "element.hpp"
#include <QString>
#include <QSet>

namespace controller
{
	class UMLDiagramCommand : public ICommand
	{
	public:
		UMLDiagramCommand(element::UMLDiagram * diagram) : _diagram(diagram) {}

		element::UMLDiagram * diagram() const					{ return _diagram; }
		QSet<element::ElementObject*> changedElements() const	{ return _changedElements; }

	protected:
		QSet<element::ElementObject *> _changedElements;

	private:
		element::UMLDiagram * _diagram;
	};

	class RenameCommand : public UMLDiagramCommand
	{
	public:
		enum Error
		{
			Error_ElementNotFound = ICommand::Error_NoError+1,
		};

		RenameCommand(element::UMLDiagram * diagram, const QString & elementQualifiedName, const QString & newName);

		virtual int redo();
		virtual int undo();

		// getters and setters
		const QString & oldQualifiedName() const	{ return _oldQualifiedName; }
		const QString & newQualifiedName() const	{ return _newQualifiedName; }
		const QString & oldName() const				{ return _oldName; }
		const QString & newName() const				{ return _newName; }

	private:
		const QString _oldQualifiedName;
		QString _newQualifiedName;
		QString _oldName;
		const QString _newName;
	};
	class MoveCommand : public UMLDiagramCommand
	{
	public:
		enum Error
		{
			Error_ElementNotFound = ICommand::Error_NoError+1,
		};

		MoveCommand(element::UMLDiagram * diagram, const QString & elementQualifiedName, const QString & newParentQualifiedName);

		virtual int redo();
		virtual int undo();

		// getters and setters
		const QString & oldQualifiedName() const			{ return _oldQualifiedName; }
		const QString & newQualifiedName() const			{ return _newQualifiedName; }
		const QString & oldParentQualifiedName() const		{ return _oldParentQualifiedName; }
		const QString & newParentQualifiedName() const		{ return _newParentQualifiedName; }

	private:
		const QString _oldQualifiedName;
		QString _newQualifiedName;
		QString _oldParentQualifiedName;
		const QString _newParentQualifiedName;
	};

	class SafeCommand : public ICommand
	{
	public:
		enum Error
		{
			Error_NotReadyForUndo = ICommand::Error_NoError + 1,
			Error_NotReadyForRedo,
			Error_SubCommandError,
			Error_CheckingElementsError,
		};

		SafeCommand(const QList<UMLDiagramCommand*> & commands);
		~SafeCommand();

		virtual int redo();
		virtual int undo();

	private:
		void undoLoop(int startPos);



	private:
		QList<UMLDiagramCommand*> _commands;
		bool _nextIsRedo;
	};
}

#endif // COMMAND_HPP
